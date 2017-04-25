//
// Created by dapl on 2017-04-20.
//

#include <cassert>
#include "TLSF.hpp"
#include "MsbLsb.hpp"
#include "BitSlice.hpp"

using namespace PiOS;

template<unsigned int SL_BITS, typename BitfieldType>
TLSF<SL_BITS, BitfieldType>::TLSF(MemorySpace memoryToManage) :
        mMetadata(*new(memoryToManage.begin()) Metadata(
                MemorySpace(static_cast<char *>(memoryToManage.begin()) + sizeof(Metadata), memoryToManage.end()))),
        mManagedMemorySpace(memoryToManage) {}

template<unsigned int SL_BITS, typename BitfieldType>
MemorySpace TLSF<SL_BITS, BitfieldType>::allocate(std::size_t size) {
    adjustSize(size);

    if (size > availableFreeMemory() or size == 0)
        return MemorySpace(nullptr, nullptr);

    auto indexes = findSuitableExistingBlock(size);

    UniversalBlock *freeBlockHeader = fetchHeader(indexes, size);
    assert(freeBlockHeader != nullptr);
    assert(freeBlockHeader->isUsed());
    assert(freeBlockHeader->size() >= size);

    return MemorySpace(freeBlockHeader->memory(),
                       static_cast<char *>(freeBlockHeader->memory()) + freeBlockHeader->size());
}

template<unsigned int SL_BITS, typename BitfieldType>
void TLSF<SL_BITS, BitfieldType>::adjustSize(std::size_t &size) const {
    if (size < MIN_BLOCK_SIZE and size > 0) size = MIN_BLOCK_SIZE;
}

template<unsigned int SL_BITS, typename BitfieldType>
typename TLSF<SL_BITS, BitfieldType>::Indexes
TLSF<SL_BITS, BitfieldType>::findSuitableExistingBlock(size_t size) {

    auto indexes = sizeToIndexes(size);
    auto sliInCurrentFli = findSliInCurrentFli(indexes);

    Indexes foundIndexes(0, 0);

    if (sliInCurrentFli.any()) {
        foundIndexes = Indexes(indexes.fli, MsbLsb().calculateLSB(sliInCurrentFli));
    } else {
        foundIndexes = findNextFreeBlocks(indexes);
        assert(foundIndexes.fli > indexes.fli);
    }

    return foundIndexes;
}

template<unsigned int SL_BITS, typename BitfieldType>
typename TLSF<SL_BITS, BitfieldType>::Indexes
TLSF<SL_BITS, BitfieldType>::findNextFreeBlocks(TLSF::Indexes indexes) {
    const auto allOnesFli = ~std::bitset<FL_SIZE>();
    auto mask = ~(allOnesFli >> (FL_SIZE - (indexes.fli + 1)));

    int fliIdx = MsbLsb().calculateLSB(mMetadata.flBitset() & mask);
    unsigned int unsignedFliIdx = static_cast<unsigned int>(fliIdx);
    assert(fliIdx >= 0);
    assert(mMetadata.slBitset(unsignedFliIdx).any());

    auto sliIdx = MsbLsb().calculateLSB(mMetadata.slBitset(unsignedFliIdx));
    assert(sliIdx >= 0);

    return Indexes(fliIdx, sliIdx);
}


template<unsigned int SL_BITS, typename BitfieldType>
std::bitset<TLSF<SL_BITS, BitfieldType>::SL_SIZE>
TLSF<SL_BITS, BitfieldType>::findSliInCurrentFli(Indexes indexes) const {
    auto sliBitset = mMetadata.slBitset(indexes.fli);
    auto sliIndex = MsbLsb().calculateLSB(sliBitset);
    const auto allOnesSli = ~std::bitset<SL_SIZE>();
    auto mask = ~(allOnesSli >> (SL_BITS - (sliIndex + 1)));

    return sliBitset & mask;
}

template<unsigned int SL_BITS, typename BitfieldType>
void TLSF<SL_BITS, BitfieldType>::deallocate(void *space) {
    if (space == nullptr)
        return;

    UsedBlockHeader *blockToDeallocate = findUsedBlock(space);
    assert(blockToDeallocate->isUsed());

    blockToDeallocate = concatenateAdjacentBlocks(blockToDeallocate);

    UsedBlockHeader blockCpy = *blockToDeallocate;
    blockToDeallocate->~UsedBlockHeader();
    createNewFreeHeader(reinterpret_cast<char *>(blockToDeallocate), blockCpy.size(), blockCpy.isLastBlock(),
                        blockCpy.previousPhysical());

}

template<unsigned int SL_BITS, typename BitfieldType>
std::size_t TLSF<SL_BITS, BitfieldType>::availableFreeMemory() {
    MsbLsb msbCalc;

    auto fli = msbCalc.calculateMSB(mMetadata.flBitset());
    assert(fli >= 0);
    unsigned int unsignedFli = static_cast<unsigned int>(fli);

    auto sli = msbCalc.calculateMSB(mMetadata.slBitset(unsignedFli).to_ulong());

    return indexesToSize(Indexes(fli, sli));
}

template<unsigned int SL_BITS, typename BitfieldType>
std::size_t TLSF<SL_BITS, BitfieldType>::indexesToSize(TLSF::Indexes indexes) {
    std::bitset<FL_SIZE> size(0);
    size.set(indexes.fli);
    size |= indexes.sli << (indexes.fli - SL_BITS);

    return static_cast<std::size_t>(size.to_ullong());
}

template<unsigned int SL_BITS, typename BitfieldType>
typename TLSF<SL_BITS, BitfieldType>::Indexes TLSF<SL_BITS, BitfieldType>::sizeToIndexes(std::size_t size) {
    assert(size > 0);

    MsbLsb msbCalc;
    BitSlice slice;

    auto fli = msbCalc.calculateMSB(size);
    assert(size >= 0);
    auto sli = slice.calculate(size, fli - SL_BITS, static_cast<unsigned int>(fli));
    return Indexes(fli, sli);
}

template<unsigned int SL_BITS, typename BitfieldType>
typename TLSF<SL_BITS, BitfieldType>::UniversalBlock *
TLSF<SL_BITS, BitfieldType>::fetchHeader(Indexes indexes, std::size_t trimSize) {
    assert(indexesToSize(indexes) >= trimSize);

    FreeBlockHeader *blockHeader = popFromQueue(indexes);
    assert(blockHeader != nullptr);

    trimBlockToSize(blockHeader, trimSize);
    auto *foundBlock = changeBlockToUsed(blockHeader);

    return foundBlock;
}

template<unsigned int SL_BITS, typename BitfieldType>
typename TLSF<SL_BITS, BitfieldType>::UsedBlockHeader *
TLSF<SL_BITS, BitfieldType>::changeBlockToUsed(FreeBlockHeader *blockHeader) const {
    FreeBlockHeader resultHeader = *blockHeader;
    blockHeader->~FreeBlockHeader();
    UsedBlockHeader *foundBlock = new(blockHeader) UniversalBlock(resultHeader.size(),
                                                                  true, resultHeader.isLastBlock(),
                                                                  resultHeader.previousPhysical());
    return foundBlock;
}

template<unsigned int SL_BITS, typename BitfieldType>
typename TLSF<SL_BITS, BitfieldType>::FreeBlockHeader *
TLSF<SL_BITS, BitfieldType>::popFromQueue(TLSF::Indexes indexes) {
    FreeBlockHeader *fetchedBlock = mMetadata.header(indexes);
    FreeBlockHeader *next = nullptr;
    if (fetchedBlock != nullptr) {
        next = fetchedBlock->nextFreeBlock();
        fetchedBlock->setNextFreeBlock(nullptr);
    }
    if (next != nullptr) {
        next->setPreviousFreeBlock(nullptr);
    }

    mMetadata.setHeader(indexes, next);
    updateBitsets(indexes);
    return fetchedBlock;
}

template<unsigned int SL_BITS, typename BitfieldType>
void TLSF<SL_BITS, BitfieldType>::trimBlockToSize(FreeBlockHeader *&blockToSplit, size_t trimSize) {
    assert(trimSize >= MIN_BLOCK_SIZE);
    assert(blockToSplit->size() >= trimSize);

    auto originalSize = blockToSplit->size();
    const auto secondBlockSize = originalSize - trimSize;
    if (secondBlockSize < MIN_BLOCK_SIZE)
        return;

    blockToSplit->setSize(trimSize);

    auto newAllocationArea = static_cast<char *>(blockToSplit->memory()) + blockToSplit->size();
    createNewFreeHeader(newAllocationArea, secondBlockSize, blockToSplit->isLastBlock(), nullptr);

    blockToSplit->setLastBlockFlag(false);
}

template<unsigned int SL_BITS, typename BitfieldType>
void TLSF<SL_BITS, BitfieldType>::createNewFreeHeader(char *blockBegin, size_t sizeOfNewBlock, bool isLast,
                                                      UniversalBlock *previousPhysical) {
    auto indexes = sizeToIndexes(sizeOfNewBlock);
    auto firstHeader = mMetadata.header(indexes);

    auto *newBlock = new(blockBegin) FreeBlockHeader(sizeOfNewBlock, isLast, previousPhysical, firstHeader, nullptr);

    if (firstHeader != nullptr)
        firstHeader->setPreviousFreeBlock(newBlock);

    mMetadata.setHeader(indexes, newBlock);
    updateBitsets(indexes);
}

template<unsigned int SL_BITS, typename BitfieldType>
void TLSF<SL_BITS, BitfieldType>::updateBitsets(const TLSF<SL_BITS, BitfieldType>::Indexes &indexes) const {

    if (mMetadata.header(indexes) == nullptr) {
        mMetadata.slBitset(indexes.fli).set(indexes.sli, 0);
        if (mMetadata.slBitset(indexes.fli).none())
            mMetadata.flBitset().set(indexes.fli, 0);
    } else {
        mMetadata.flBitset().set(indexes.fli);
        mMetadata.slBitset(indexes.fli).set(indexes.sli);
    }
}

template<unsigned int SL_BITS, typename BitfieldType>
typename TLSF<SL_BITS, BitfieldType>::UsedBlockHeader *TLSF<SL_BITS, BitfieldType>::findUsedBlock(void *space) {
    return reinterpret_cast<UsedBlockHeader *>(static_cast<char *>(space) -
                                               sizeof(UsedBlockHeader));
}

template<unsigned int SL_BITS, typename BitfieldType>
typename TLSF<SL_BITS, BitfieldType>::UsedBlockHeader *
TLSF<SL_BITS, BitfieldType>::concatenateAdjacentBlocks(UsedBlockHeader *centerBlock) {
    assert(centerBlock != nullptr);

    auto previousBlock = centerBlock->previousPhysical();
    auto nextBlock = centerBlock->nextPhysical();

    if (isMergeable(previousBlock))
        mergeTwoBlocks(centerBlock, reinterpret_cast<FreeBlockHeader *>(previousBlock));
    if (isMergeable(nextBlock))
        mergeTwoBlocks(centerBlock, reinterpret_cast<FreeBlockHeader *>(nextBlock));

    return centerBlock;
}

template<unsigned int SL_BITS, typename BitfieldType>
void
TLSF<SL_BITS, BitfieldType>::mergeTwoBlocks(UsedBlockHeader *&centerBlock, FreeBlockHeader *otherBlock) {
    assert(centerBlock->nextPhysical() == otherBlock or centerBlock->previousPhysical() == otherBlock);

    removeFromQueue(otherBlock);

    std::size_t newBlockSize = centerBlock->size() + otherBlock->size() + sizeof(UniversalBlock);
    if (centerBlock->nextPhysical() == otherBlock) {
        centerBlock->setSize(newBlockSize);
        otherBlock->~FreeBlockHeader();
    } else {
        UniversalBlock freeBlockCpy(otherBlock->size(), false, otherBlock->isLastBlock(),
                                    otherBlock->previousPhysical());
        otherBlock->~FreeBlockHeader();
        centerBlock->~UsedBlockHeader();
        centerBlock = new(otherBlock) UniversalBlock(newBlockSize, true,
                                                     freeBlockCpy.isLastBlock(), freeBlockCpy.previousPhysical());
    }
}

template<unsigned int SL_BITS, typename BitfieldType>
void
TLSF<SL_BITS, BitfieldType>::removeFromQueue(const TLSF<SL_BITS, BitfieldType>::FreeBlockHeader *blockToRemove) const {
    if (blockToRemove->previousFreeBlock() != nullptr)
        blockToRemove->previousFreeBlock()->setNextFreeBlock(blockToRemove->nextFreeBlock());
    else {
        Indexes indexes = sizeToIndexes(blockToRemove->size());
        mMetadata.setHeader(indexes, blockToRemove->nextFreeBlock());
    }
}

template<unsigned int SL_BITS, typename BitfieldType>
bool TLSF<SL_BITS, BitfieldType>::isMergeable(const TLSF<SL_BITS, BitfieldType>::UniversalBlock *previousBlock) const {
    return previousBlock != nullptr and not previousBlock->isUsed();
}

template<unsigned int SL_BITS, typename BitfieldType>
class TLSF<SL_BITS, BitfieldType>::UniversalBlock {
    size_t mSize;
    UniversalBlock *mPreviousBlock;
public:
    UniversalBlock(size_t blockSize, bool isUsed, bool isLast, UniversalBlock *previousBlock)
            : mSize((blockSize << 2) | (isUsed << 1) | isLast),
              mPreviousBlock(previousBlock) {}

    size_t size() const { return (std::bitset<32>(mSize) >> 2).to_ulong(); }

    void setSize(size_t size) { mSize = (size << 2) | (mSize & 0b11u); }

    UniversalBlock *previousPhysical() const { return mPreviousBlock; }

    UniversalBlock *nextPhysical() {
        return reinterpret_cast<UniversalBlock *>(static_cast<char *>(memory()) + size());
    }

    bool isLastBlock() const { return static_cast<bool>((mSize & 1u)); }

    void setLastBlockFlag(bool flagValue) { mSize = std::bitset<32>(mSize).set(0, flagValue).to_ulong(); }

    bool isUsed() const { return (mSize & 0b10u) != 0u; }

    void *memory() { return reinterpret_cast<char *>(this) + sizeof(*this); }
};

template<unsigned int SL_BITS, typename BitfieldType>
class TLSF<SL_BITS, BitfieldType>::FreeBlockHeader : public UniversalBlock {
    FreeBlockHeader *mNext;
    FreeBlockHeader *mPrevious;
public:
    FreeBlockHeader(std::size_t blockSize, bool isLast, UniversalBlock *previousPhysBlock,
                    FreeBlockHeader *next,
                    FreeBlockHeader *previous) :
            UniversalBlock(blockSize, false, isLast, previousPhysBlock),
            mNext(next),
            mPrevious(previous) {}

    FreeBlockHeader *nextFreeBlock() const { return mNext; }

    void setNextFreeBlock(FreeBlockHeader *freeBlock) { mNext = freeBlock; }

    FreeBlockHeader *previousFreeBlock() const { return mPrevious; }

    void setPreviousFreeBlock(FreeBlockHeader *freeBlock) { mPrevious = freeBlock; }

};

template<unsigned int SL_BITS, typename BitfieldType>
class TLSF<SL_BITS, BitfieldType>::Metadata {
public:
    using FlIdx = unsigned int;
    using SlIdx = unsigned int;

    Metadata(MemorySpace memoryToManage) :
            mInfo(),
            mFlBitset(),
            mSlBitsets() {
        auto memorySize = static_cast<char *>(memoryToManage.end()) - static_cast<char *>(memoryToManage.begin());
        auto indexes = sizeToIndexes(memorySize);
        mFlBitset.set(indexes.fli);
        mSlBitsets[indexes.fli].set(indexes.sli);

        for (auto &sl : mInfo) {
            for (auto &block : sl) {
                block = nullptr;
            }
        }

        const auto effectiveMemorySize =
                static_cast<char *>(memoryToManage.end()) - static_cast<char *>(memoryToManage.begin()) -
                sizeof(UniversalBlock);
        auto emptyBlock = new(memoryToManage.begin()) FreeBlockHeader(effectiveMemorySize, true, nullptr, nullptr,
                                                                      nullptr);

        mInfo[indexes.fli][indexes.sli] = emptyBlock;
    }

    std::bitset<FL_SIZE> &flBitset() {
        return mFlBitset;
    }

    std::bitset<SL_SIZE> &slBitset(FlIdx FlIdx) {
        return mSlBitsets[FlIdx];
    };

    FreeBlockHeader *header(Indexes indexes) {
        return mInfo[indexes.fli][indexes.sli];
    }

    void setHeader(Indexes indexes, FreeBlockHeader *header) {
        mInfo[indexes.fli][indexes.sli] = header;
    }

private:
    using TwoLevelInfo = std::array<std::array<FreeBlockHeader *, SL_SIZE>, FL_SIZE>;
    TwoLevelInfo mInfo;
    std::bitset<FL_SIZE> mFlBitset;
    std::array<std::bitset<SL_SIZE>, FL_SIZE> mSlBitsets;
};
