////
//// Created by dapl on 2017-04-20.
////
//
//#include <cassert>
//#include "TLSF.hpp"
//#include "MsbLsbCalculator.hpp"
//
//template<size_t SL_SIZE, typename BitfieldType = uint32_t>
//PiOS::TLSF<SL_SIZE, BitfieldType>::TLSF(PiOS::MemorySpace memoryToManage) :
//        mMetadata(*(new(memoryToManage.begin()) Metadata(memoryToManage))) {
//    assert(memoryToManage.size() > sizeof(Metadata));
//}
//
//template<size_t SL_SIZE, typename BitfieldType = uint32_t>
//PiOS::MemorySpace PiOS::TLSF::allocate(size_t size) {
//    return MemorySpace(nullptr, nullptr);
//}
//
//void PiOS::TLSF::deallocate(void *spaceBegin) {
//
//}
//
//
//template<size_t SL_SIZE, typename BitfieldType = uint32_t>
//PiOS::TLSF<SL_SIZE, BitfieldType>::Metadata::Metadata(MemorySpace memoryToManage) {
//}
