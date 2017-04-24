//
// Created by dapl on 2017-04-20.
//

#include <cassert>
#include "TLSF.hpp"
#include "MsbLsbCalculator.hpp"
#include "BitSliceCalculator.hpp"

using namespace PiOS;

template<unsigned int SL_BITS, typename BitfieldType>
TLSF<SL_BITS, BitfieldType>::TLSF(MemorySpace memoryToManage) :
        mMetadata(*new(memoryToManage.begin()) Metadata(
                MemorySpace(static_cast<char *>(memoryToManage.begin()) + sizeof(Metadata), memoryToManage.end()))),
        mManagedMemorySpace(memoryToManage) {}

template<unsigned int SL_BITS, typename BitfieldType>
MemorySpace TLSF<SL_BITS, BitfieldType>::allocate(size_t size) {
    size = adjustSize(size);

    if (size > availableFreeMemory())
        return MemorySpace(nullptr, nullptr);

    auto indexes = sizeToIndexes(size);
    indexes = findSuitableExistingBlock(indexes);

    UniversalBlock &freeBlockHeader = fetchHeader(indexes, size);
    freeBlockHeader.setUsedFlag(true);

    return MemorySpace(freeBlockHeader.memory(),
                       static_cast<char *>(freeBlockHeader.memory()) + freeBlockHeader.size());
}

template<unsigned int SL_BITS, typename BitfieldType>
size_t TLSF<SL_BITS, BitfieldType>::adjustSize(size_t size) const {
    if (size < MIN_BLOCK_SIZE)
        size = MIN_BLOCK_SIZE;
    return size;
}

template<unsigned int SL_BITS, typename BitfieldType>
typename TLSF<SL_BITS, BitfieldType>::Indexes
TLSF<SL_BITS, BitfieldType>::findSuitableExistingBlock(Indexes preferableIndexes) {
    std::bitset<FL_SIZE> fliBitset(preferableIndexes.fli);
    std::bitset<FL_SIZE> mask = ~((~std::bitset<FL_SIZE>()) >> (FL_SIZE - preferableIndexes.fli));
    MsbLsbCalculator lsbCalculator;

    int smallestAvailableFli = lsbCalculator.calculateLSB(mMetadata.flBitset() & mask);
    assert(smallestAvailableFli >= 0);

    auto sliBitset = mMetadata.slBitsetForFl(smallestAvailableFli);
    int smallestAvailableSli = lsbCalculator.calculateLSB(sliBitset);
    return TLSF::Indexes(smallestAvailableFli, smallestAvailableSli);
}

template<unsigned int SL_BITS, typename BitfieldType>
void TLSF<SL_BITS, BitfieldType>::deallocate(void *) {

}

template<unsigned int SL_BITS, typename BitfieldType>
size_t TLSF<SL_BITS, BitfieldType>::availableFreeMemory() {
    MsbLsbCalculator msbCalc;
    auto fli = msbCalc.calculateMSB(mMetadata.flBitset());
    auto sli = msbCalc.calculateMSB(mMetadata.slBitsetForFl(fli).to_ulong());

    return indexesToSize(Indexes(fli, sli));
}

template<unsigned int SL_BITS, typename BitfieldType>
size_t TLSF<SL_BITS, BitfieldType>::indexesToSize(TLSF::Indexes indexes) {
    std::bitset<FL_SIZE> size;
    size.set(indexes.fli);
    size |= indexes.sli << (indexes.fli - SL_BITS);

    return size.to_ulong();
}

template<unsigned int SL_BITS, typename BitfieldType>
typename TLSF<SL_BITS, BitfieldType>::Indexes TLSF<SL_BITS, BitfieldType>::sizeToIndexes(size_t size) {
    MsbLsbCalculator msbCalc;
    BitSliceCalculator mSlice;
    auto fli = msbCalc.calculateMSB(size);
    auto sli = mSlice.calculate(size, fli - SL_BITS, fli);
    return Indexes(fli, sli);
}

template<unsigned int SL_BITS, typename BitfieldType>
bool TLSF<SL_BITS, BitfieldType>::isBlockAvailable(TLSF::Indexes indexes) {
    bool fliAvailable = mMetadata.flBitset().test(indexes.fli);
    bool sliAvailable = mMetadata.slBitsetForFl(indexes.fli).test(indexes.sli);
    return fliAvailable and sliAvailable;
}

template<unsigned int SL_BITS, typename BitfieldType>
typename TLSF<SL_BITS, BitfieldType>::UniversalBlock &
TLSF<SL_BITS, BitfieldType>::fetchHeader(Indexes indexes, size_t trimSize) {

    FreeBlockHeader *blockHeader = mMetadata.slForFl(indexes.fli)[indexes.sli];
    assert(blockHeader != nullptr);

    FreeBlockHeader *nextFreeBlock = blockHeader->nextFreeBlock();
    mMetadata.slForFl(indexes.fli)[indexes.sli] = nextFreeBlock;

    updateBitsets(indexes, nextFreeBlock);
    splitBlockToSize(blockHeader, trimSize);

    FreeBlockHeader resultHeader = *blockHeader;

    blockHeader->~FreeBlockHeader();

    return *new(blockHeader) UniversalBlock(resultHeader.size(),
                                            false, resultHeader.isLastBlock(), resultHeader.previousPhysical());
}

template<unsigned int SL_BITS, typename BitfieldType>
void TLSF<SL_BITS, BitfieldType>::splitBlockToSize(FreeBlockHeader *&blockToSplit, size_t size) {
    assert(size >= MIN_BLOCK_SIZE);

    auto originalSize = blockToSplit->size();
    const auto secondBlockSize = originalSize - size;
    if (secondBlockSize < MIN_BLOCK_SIZE)
        return;

    blockToSplit->setSize(size);

    auto indexes = sizeToIndexes(secondBlockSize);
    FreeBlockHeader *firstHeaderOfNewSize = mMetadata.slForFl(indexes.fli)[indexes.sli];
    auto newAllocationArea = static_cast<char *>(blockToSplit->memory()) + blockToSplit->size();

    auto newHeader = new(newAllocationArea) FreeBlockHeader(secondBlockSize, blockToSplit->isLastBlock(),
                                                            blockToSplit, firstHeaderOfNewSize, nullptr);
    firstHeaderOfNewSize->setPreviousFreeBlock(newHeader);

    mMetadata.slForFl(indexes.fli)[indexes.sli] = newHeader;
}

template<unsigned int SL_BITS, typename BitfieldType>
void TLSF<SL_BITS, BitfieldType>::updateBitsets(const TLSF<SL_BITS, BitfieldType>::Indexes &indexes,
                                                const TLSF<SL_BITS, BitfieldType>::FreeBlockHeader *nextFreeBlock) const {
    if (nextFreeBlock == nullptr) {
        mMetadata.slBitsetForFl(indexes.fli).set(indexes.sli, 0);
        if (mMetadata.slBitsetForFl(indexes.fli).none())
            mMetadata.flBitset().set(indexes.fli, 0);
    }
}

template<unsigned int SL_BITS, typename BitfieldType>
class TLSF<SL_BITS, BitfieldType>::Metadata {
public:
    using FlIdx = unsigned int;

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

    std::bitset<SL_SIZE> &slBitsetForFl(FlIdx FlIdx) {
        return mSlBitsets[FlIdx];
    }

    std::array<FreeBlockHeader *, SL_SIZE> &slForFl(FlIdx FlIdx) {
        return mInfo[FlIdx];
    };

private:
    using TwoLevelInfo = std::array<std::array<FreeBlockHeader *, SL_SIZE>, FL_SIZE>;
    TwoLevelInfo mInfo;
    std::bitset<FL_SIZE> mFlBitset;
    std::array<std::bitset<SL_SIZE>, FL_SIZE> mSlBitsets;
};
