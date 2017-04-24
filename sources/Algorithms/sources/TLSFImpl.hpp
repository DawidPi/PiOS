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

    if (size > availableFreeMemory())
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
    if (size < MIN_BLOCK_SIZE) size = MIN_BLOCK_SIZE;
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
    auto mask = allOnesFli >> (FL_SIZE - indexes.fli);
    mask ^= allOnesFli;

    auto fliIdx = MsbLsb().calculateLSB(mMetadata.flBitset() & mask);
    assert(fliIdx >= 0);
    assert(mMetadata.slBitsetForFl(fliIdx).any());

    auto sliIdx = MsbLsb().calculateLSB(mMetadata.slBitsetForFl(fliIdx));
    assert(sliIdx >= 0);

    return Indexes(fliIdx, sliIdx);
}


template<unsigned int SL_BITS, typename BitfieldType>
std::bitset<SL_BITS> TLSF<SL_BITS, BitfieldType>::findSliInCurrentFli(Indexes indexes) const {
    auto sliBitset = mMetadata.slBitsetForFl(indexes.fli);
    auto sliIndex = MsbLsb().calculateLSB(sliBitset);
    const auto allOnesSli = ~std::bitset<SL_BITS>();
    auto mask = allOnesSli >> (SL_BITS - sliIndex);
    mask ^= allOnesSli;

    return sliBitset & mask;
}

template<unsigned int SL_BITS, typename BitfieldType>
void TLSF<SL_BITS, BitfieldType>::deallocate(void *) {

}

template<unsigned int SL_BITS, typename BitfieldType>
std::size_t TLSF<SL_BITS, BitfieldType>::availableFreeMemory() {
    MsbLsb msbCalc;
    auto fli = msbCalc.calculateMSB(mMetadata.flBitset());
    auto sli = msbCalc.calculateMSB(mMetadata.slBitsetForFl(fli).to_ulong());

    return indexesToSize(Indexes(fli, sli));
}

template<unsigned int SL_BITS, typename BitfieldType>
std::size_t TLSF<SL_BITS, BitfieldType>::indexesToSize(TLSF::Indexes indexes) {
    std::bitset<FL_SIZE> size;
    size.set(indexes.fli);
    size |= indexes.sli << (indexes.fli - SL_BITS);

    return size.to_ulong();
}

template<unsigned int SL_BITS, typename BitfieldType>
typename TLSF<SL_BITS, BitfieldType>::Indexes TLSF<SL_BITS, BitfieldType>::sizeToIndexes(std::size_t size) {
    assert(size > 0);

    MsbLsb msbCalc;
    BitSlice slice;

    auto fli = msbCalc.calculateMSB(size);
    auto sli = slice.calculate(size, fli - SL_BITS, fli);
    return Indexes(fli, sli);
}

template<unsigned int SL_BITS, typename BitfieldType>
bool TLSF<SL_BITS, BitfieldType>::isBlockAvailable(TLSF::Indexes indexes) {
    bool fliAvailable = mMetadata.flBitset().test(indexes.fli);
    bool sliAvailable = mMetadata.slBitsetForFl(indexes.fli).test(indexes.sli);
    return fliAvailable and sliAvailable;
}

template<unsigned int SL_BITS, typename BitfieldType>
typename TLSF<SL_BITS, BitfieldType>::UniversalBlock *
TLSF<SL_BITS, BitfieldType>::fetchHeader(Indexes indexes, std::size_t trimSize) {

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
void TLSF<SL_BITS, BitfieldType>::trimBlockToSize(FreeBlockHeader *&blockToSplit, size_t size) {
    assert(size >= MIN_BLOCK_SIZE);

    auto originalSize = blockToSplit->size();
    const auto secondBlockSize = originalSize - size;
    if (secondBlockSize < MIN_BLOCK_SIZE)
        return;

    blockToSplit->setSize(size);

    auto indexes = sizeToIndexes(secondBlockSize);
    FreeBlockHeader *firstHeaderOfNewSize = mMetadata.header(indexes);
    auto newAllocationArea = static_cast<char *>(blockToSplit->memory()) + blockToSplit->size();

    auto *newHeader = new(newAllocationArea) FreeBlockHeader(secondBlockSize, blockToSplit->isLastBlock(),
                                                             blockToSplit, firstHeaderOfNewSize, nullptr);
    blockToSplit->setLastBlockFlag(false);
    firstHeaderOfNewSize->setPreviousFreeBlock(newHeader);

    mMetadata.setHeader(indexes, newHeader);
}

template<unsigned int SL_BITS, typename BitfieldType>
void TLSF<SL_BITS, BitfieldType>::updateBitsets(const TLSF<SL_BITS, BitfieldType>::Indexes &indexes) const {

    if (mMetadata.header(indexes) == nullptr) {
        mMetadata.slBitsetForFl(indexes.fli).set(indexes.sli, 0);
        if (mMetadata.slBitsetForFl(indexes.fli).none())
            mMetadata.flBitset().set(indexes.fli, 0);
    }
}

template<unsigned int SL_BITS, typename BitfieldType>
class TLSF<SL_BITS, BitfieldType>::Metadata {
public:
    using FlIdx = unsigned int;
    using SlIdx = unsigned int;

    Metadata(MemorySpace memoryToManage) :
            mInfo(),
            mFlBitset(),
            mSlBitsets() {
        auto memorySize = static_cast<char *>(memoryToManage.begin()) - static_cast<char *>(memoryToManage.end());
        auto indexes = sizeToIndexes(memorySize);
        mFlBitset.set(indexes.fli);
        mSlBitsets[indexes.fli].set(indexes.sli);

        for (auto &sl : mInfo)
            for (auto &block : sl) {
                block = nullptr;
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

    std::bitset<SL_BITS> &slBitsetForFl(FlIdx FlIdx) {
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
    std::array<std::bitset<SL_BITS>, FL_SIZE> mSlBitsets;
};