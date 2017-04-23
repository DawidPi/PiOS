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
                MemorySpace(memoryToManage.begin() + sizeof(Metadata), memoryToManage.end()))),
        mManagedMemorySpace(memoryToManage) {}

template<unsigned int SL_BITS, typename BitfieldType>
MemorySpace TLSF<SL_BITS, BitfieldType>::allocate(size_t size) {
    if (size < MIN_BLOCK_SIZE)
        size = MIN_BLOCK_SIZE;

    if (size > availableFreeMemory())
        return MemorySpace(nullptr, nullptr);

    auto indexes = sizeToIndexes(size);
    if (isAvailable(indexes)) {
        FreeBlockHeader *freeBlockHeader = fetchHeader(indexes);

    }

}

template<unsigned int SL_BITS, typename BitfieldType>
void TLSF<SL_BITS, BitfieldType>::deallocate(void *spaceBegin) {

}

template<unsigned int SL_BITS, typename BitfieldType>
size_t TLSF<SL_BITS, BitfieldType>::availableFreeMemory() {
    MsbLsbCalculator msbCalc;
    auto fli = msbCalc.calculateMSB(mMetadata.flBitset());
    auto sli = msbCalc.calculateMSB(mMetadata.slBitsetForFl(fli));

    return indexesToSize(Indexes(fli, sli));
}

template<unsigned int SL_BITS, typename BitfieldType>
size_t TLSF<SL_BITS, BitfieldType>::indexesToSize(TLSF::Indexes indexes) {
    std::bitset<FL_SIZE> size;
    size.set(indexes.fli);
    size |= indexes.sli << (indexes.fli - SL_BITS);

    return size.to_ullong();
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
bool TLSF<SL_BITS, BitfieldType>::isAvailable(TLSF::Indexes indexes) {
    bool fliAvailable = mMetadata.flBitset().test(indexes.fli);
    bool sliAvailable = mMetadata.slBitsetForFl(indexes.fli).test(indexes.sli);
    return fliAvailable and sliAvailable;
}

template<unsigned int SL_BITS, typename BitfieldType>
typename TLSF<SL_BITS, BitfieldType>::FreeBlockHeader *TLSF<SL_BITS, BitfieldType>::fetchHeader(TLSF::Indexes indexes) {
    //todo implement
    return nullptr;
}

template<unsigned int SL_BITS, typename BitfieldType>
class TLSF<SL_BITS, BitfieldType>::Metadata {
public:
    using FlIdx = unsigned int;

    Metadata(MemorySpace memoryToManage) :
            mInfo(),
            mFlBitset(),
            mSlBitsets() {
        MsbLsbCalculator fliCalculator;
        BitSliceCalculator sliCalculator;
        auto memorySize = static_cast<char *>(memoryToManage.begin()) - static_cast<char *>(memoryToManage.end());
        auto indexes = sizeToIndexes(memorySize);
        mFlBitset.set(indexes.fli);
        mSlBitsets[indexes.fli].set(indexes.sli);

        for (auto &sl : mInfo)
            for (auto &block : sl) {
                block = nullptr;
            }

        auto emptyBlock = new(memoryToManage.begin()) FreeBlockHeader();
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
