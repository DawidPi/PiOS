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
#include "MsbLsb.hpp"

namespace PiOS {
    /*!
     * \brief Implementation of Two Level Segregate Fit algorithm for dynamic
     * memory allocation and deallocation.
     */
    template<unsigned int SL_BITS = 3, typename BitfieldType = uint32_t>
    class TLSF : public DynamicAllocator {
        constexpr static unsigned int roundBlockSize(unsigned int size) {
            MsbLsb msbCalc;
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
            UniversalBlock(size_t blockSize, bool isUsed, bool isLast, UniversalBlock *previousBlock)
                    : mSize((blockSize << 2) | (isUsed << 1) | isLast),
                      mPreviousBlock(previousBlock) {}

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

            void *memory() { return reinterpret_cast<char *>(this) + sizeof(*this); }
        };

        class FreeBlockHeader : public UniversalBlock {
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

        using UsedBlockHeader = UniversalBlock;

        static constexpr std::size_t FL_SIZE = CHAR_BIT * sizeof(BitfieldType);
        static constexpr std::size_t SL_SIZE = pow(2, SL_BITS);
        static constexpr std::size_t MIN_BLOCK_SIZE = roundBlockSize(sizeof(FreeBlockHeader));
        static constexpr std::size_t MIN_BLOCK_SIZE_POW = MsbLsb().calculateMSB(MIN_BLOCK_SIZE);
        static_assert(sizeof(FreeBlockHeader) <= MIN_BLOCK_SIZE,
                      "sizeOf FreeBlockHeader must be smaller or equal MIN_BLOCK_SIZE");
    public:
        TLSF(MemorySpace memoryToManage);

        MemorySpace allocate(std::size_t size) override;

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

        static std::size_t indexesToSize(Indexes);

        static Indexes sizeToIndexes(std::size_t size);

        std::size_t availableFreeMemory();

        bool isBlockAvailable(Indexes indexes);

        UniversalBlock *fetchHeader(Indexes indexes, std::size_t trimSize);

        Indexes findSuitableExistingBlock(size_t size);

        void adjustSize(std::size_t &size) const;

        void updateBitsets(const TLSF<SL_BITS, BitfieldType>::Indexes &indexes) const;

        void trimBlockToSize(FreeBlockHeader *&blockToSplit, std::size_t size);

        FreeBlockHeader *popFromQueue(Indexes indexes);

        UsedBlockHeader *changeBlockToUsed(FreeBlockHeader *blockHeader) const;

        std::bitset<SL_BITS> findSliInCurrentFli(Indexes indexes) const;

        Indexes findNextFreeBlocks(Indexes indexes);
    };
}

#include "TLSFImpl.hpp"

#endif //PROJECT_TLSF_HPP
