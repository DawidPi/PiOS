//
// Created by dapl on 2017-04-20.
//

#include <cassert>
#include "TLSF.hpp"

template<size_t SL_SIZE, typename BitfieldType = uint32_t>
PiOS::TLSF<SL_SIZE, BitfieldType>::TLSF(PiOS::MemorySpace memoryToManage) :
        mMetadata(*(new(memoryToManage.begin()) Metadata(memoryToManage))) {
    assert(memoryToManage.size() > sizeof(Metadata));
}

template<size_t SL_SIZE, typename BitfieldType = uint32_t>
PiOS::MemorySpace PiOS::TLSF::allocate(size_t size) {
    return MemorySpace(nullptr, nullptr);
}

void PiOS::TLSF::deallocate(void *spaceBegin) {

}

template<size_t SL_SIZE, typename BitfieldType = uint32_t>
template<typename ArgType>
size_t PiOS::TLSF<SL_SIZE, BitfieldType>::msbSetIdx(ArgType value) {
    //todo optimize me
    size_t idx = 0;
    while (value >>= 1)
        idx++;

    return idx;
}

template<size_t SL_SIZE, typename BitfieldType = uint32_t>
template<typename ArgType>
size_t PiOS::TLSF<SL_SIZE, BitfieldType>::calculateFistLevelIdx(ArgType value) {
    return msbSetIdx(value);
}

template<size_t SL_SIZE, typename BitfieldType = uint32_t>
template<typename ArgType>
size_t PiOS::TLSF<SL_SIZE, BitfieldType>::calculateSecondLevelIdx(ArgType value) {
    //todo optimize me
    auto fli = calculateFistLevelIdx(value);
    auto result = (value - pow(2, fli)) * (pow(2, SL_SIZE) / pow(2, fli));
    return result;
}


template<size_t SL_SIZE, typename BitfieldType = uint32_t>
PiOS::TLSF<SL_SIZE, BitfieldType>::Metadata::Metadata(MemorySpace memoryToManage) {

}
