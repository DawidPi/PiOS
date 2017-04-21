////
//// Created by dapl on 2017-04-20.
////
//
//#ifndef PROJECT_TLSF_HPP
//#define PROJECT_TLSF_HPP
//
//#include <cstddef>
//#include <array>
//#include <bitset>
//#include <climits>
//#include "DynamicAllocator.hpp"
//#include "power.hpp"
//#include "MSBLookUp.hpp"
//
//namespace PiOS {
//    /*!
//     * \brief Implementation of Two Level Segregate Fit algorithm for dynamic
//     * memory allocation and deallocation.
//     */
//    template<size_t SL_SIZE = pow(2, 3), typename BitfieldType = uint32_t>
//    class TLSF : public DynamicAllocator {
//    public:
//        TLSF(MemorySpace memoryToManage);
//
//        MemorySpace allocate(size_t size) override;
//
//        void deallocate(void *spaceBegin) override;
//
//    private:
//        class Metadata;
//        class FreeBlockHeader {
//
//        };
//
//        class UsedBlockHeader {
//
//        };
//
//        Metadata &mMetadata;
//    };
//
//    template<size_t SL_SIZE, typename BitfieldType>
//    class TLSF<SL_SIZE, BitfieldType>::Metadata {
//    public:
//        using FlIdx = unsigned int;
//
//        Metadata(MemorySpace memoryToManage);
//
//        std::bitset<FL_SIZE> &flBitset();
//
//        std::bitset<SL_SIZE> &slBitsetForFl(FlIdx FlIdx);
//
//        std::array<FreeBlockHeader *, SL_SIZE> &slForFl(FlIdx FlIdx);
//
//    private:
//        using TwoLevelMetadata = std::array<std::array<FreeBlockHeader *, SL_SIZE>, FL_SIZE>;
//        constexpr unsigned int FL_SIZE = CHAR_BIT * sizeof(BitfieldType);
//        TwoLevelMetadata mMetadata;
//        std::bitset<FL_SIZE> mFlBitset;
//        std::array<std::bitset<SL_SIZE>, FL_SIZE> mSlBitsets;
//    };
//}
//
//#endif //PROJECT_TLSF_HPP
