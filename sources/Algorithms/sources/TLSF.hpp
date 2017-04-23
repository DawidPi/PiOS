//
// Created by dapl on 2017-04-20.
//

#ifndef PROJECT_TLSF_HPP
#define PROJECT_TLSF_HPP

#include <cstddef>
#include <array>
#include <bitset>
#include <climits>
#include "DynamicAllocator.hpp"
#include "power.hpp"
#include "MsbLsbCalculator.hpp"

namespace PiOS {
    /*!
     * \brief Implementation of Two Level Segregate Fit algorithm for dynamic
     * memory allocation and deallocation.
     */
    template<unsigned int SL_BITS = 3, typename BitfieldType = uint32_t>
    class TLSF : public DynamicAllocator {
        constexpr static unsigned int roundBlockSize(unsigned int size) {
            MsbLsbCalculator msbCalc;
            auto idx = msbCalc.calculateMSB(size);
            idx = std::max(idx, 0);
            if (static_cast<unsigned int>(1 << idx) == size)
                return size;

            return static_cast<unsigned int>(1 << (idx + 1));
        }

        class UniversalBlock {
            size_t mSize;
            UniversalBlock *mPreviousBlock;
        public:
            size_t size() const { return (std::bitset<32>(mSize) >> 2).to_ulong(); }

            void setSize(size_t size) { mSize = (size << 2) | (mSize & 0b11u); }

            UniversalBlock *previousPhysical() const { return mPreviousBlock; }

            void setPreviousPhysical(UniversalBlock *block) { mPreviousBlock = block; }

            UniversalBlock *nextPhysical() const {
                return static_cast<UniversalBlock *>(static_cast<char *>(this) + size());
            }

            bool isLastBlock() const { return static_cast<bool>((mSize & 1u)); }

            void setLastBlockFlag(bool flagValue) { mSize = std::bitset<32>(mSize).set(0, flagValue).to_ulong(); }

            bool isUsed() const { return (mSize & 0b10u) != 0u; }

            void setUsedFlag(bool flagValue) { mSize = std::bitset<32>(mSize).set(1, flagValue).to_ulong(); }

            void *memory() { return static_cast<char *>(this) + sizeof(*this); }
        };

        class FreeBlockHeader : public UniversalBlock {
            FreeBlockHeader *mNext;
            FreeBlockHeader *mPrevious;
        public:
            FreeBlockHeader *nextFreeBlock() const { return mNext; }

            void setNextFreeBlock(FreeBlockHeader *freeBlock) { mNext = freeBlock; }

            FreeBlockHeader *previousFreeBlock() const { return mPrevious; }

            void setPreviousFreeBlock(FreeBlockHeader *freeBlock) { mPrevious = freeBlock; }
        };

        class UsedBlockHeader : public UniversalBlock {
        };

        static constexpr unsigned int FL_SIZE = CHAR_BIT * sizeof(BitfieldType);
        static constexpr unsigned int SL_SIZE = pow(2, SL_BITS);
        static constexpr unsigned int MIN_BLOCK_SIZE = roundBlockSize(sizeof(FreeBlockHeader));
        static constexpr unsigned int MIN_BLOCK_SIZE_POW = MsbLsbCalculator().calculateMSB(MIN_BLOCK_SIZE);
        static_assert(sizeof(FreeBlockHeader) <= MIN_BLOCK_SIZE,
                      "sizeOf FreeBlockHeader must be smaller or equal MIN_BLOCK_SIZE");
    public:
        TLSF(MemorySpace memoryToManage);

        MemorySpace allocate(size_t size) override;

        void deallocate(void *spaceBegin) override;

    private:
        class Metadata;

        Metadata &mMetadata;
        MemorySpace mManagedMemorySpace;
    private:
        struct Indexes {
            Indexes(BitfieldType fli_, BitfieldType sli_) : fli(fli_), sli(sli_) {}

            BitfieldType fli;
            BitfieldType sli;
        };

        static size_t indexesToSize(Indexes);

        static Indexes sizeToIndexes(size_t size);

        size_t availableFreeMemory();

        bool isAvailable(Indexes indexes);

        FreeBlockHeader *fetchHeader(Indexes indexes);
    };
}

#include "TLSFImpl.hpp"

#endif //PROJECT_TLSF_HPP
