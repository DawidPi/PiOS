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
     *
     * Implementation assures constant time allocation and deallocation.
     *
     * \tparam SL_BITS bits, that are to be taken, when calculating SL IDX
     * \tparam type of integer, that is big enough to capture size of the biggest managed block.
     */
    template<unsigned int SL_BITS = 3, typename BitfieldType = std::size_t>
    class TLSF : public DynamicAllocator {
    public:
        /*!
         * \brief Constructor.
         *
         * Initializes TLSF algorithm with amount of managed memory.
         *
         * \param memoryToManage MemorySpace representing managed memory heap.
         */
        TLSF(MemorySpace memoryToManage);

        /*!
         * \brief Allocates chunk of memory, which is no smaller than provided memory size.
         *
         * \param size Optimal size of memory to be allocated.
         *
         * If size if 0 or allocation cannot be performed, then nullptr MemorySpace is provided.
         *
         * \return MemorySpace representing block of allocated memory.
         */
        MemorySpace allocate(std::size_t size) override;

        /*!
         * \brief Deallocates chunk of memory previously allocated using \ref allocate method.
         * \param spaceBegin beginning of the space, that was previously allocated.
         *
         * If spaceBegin is nullptr, then method immediatelly returns.
         */
        void deallocate(void *spaceBegin) override;

        /*!
         * Allows to find minimum fragmentation size required to store metadata.
         * @return sizeof(Metadata)
         */
        static constexpr std::size_t metadataSize() { return sizeof(Metadata); }
    private:
        static constexpr std::size_t FL_SIZE = CHAR_BIT * sizeof(BitfieldType);
        static constexpr std::size_t SL_SIZE = pow(2, SL_BITS);

        constexpr static unsigned int roundBlockSize(unsigned int size) {
            MsbLsb msbCalc = MsbLsb();
            auto idx = msbCalc.calculateMSB(size);
            idx = std::max(idx, 0);
            if (static_cast<unsigned int>(1 << idx) == size)
                return size;

            return static_cast<unsigned int>(1 << (idx + 1));
        }

        class UniversalBlock;
        using UsedBlockHeader = UniversalBlock;

        class FreeBlockHeader;

        static constexpr std::size_t MIN_BLOCK_SIZE = roundBlockSize(sizeof(FreeBlockHeader));
        static_assert(sizeof(FreeBlockHeader) <= MIN_BLOCK_SIZE,
                      "sizeOf FreeBlockHeader must be smaller or equal MIN_BLOCK_SIZE");

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

        UniversalBlock *fetchHeader(Indexes indexes, std::size_t trimSize);
        Indexes findSuitableExistingBlock(size_t size);
        void adjustSize(std::size_t &size) const;
        void updateBitsets(const TLSF<SL_BITS, BitfieldType>::Indexes &indexes) const;

        void trimBlockToSize(FreeBlockHeader *&blockToSplit, std::size_t trimSize);

        FreeBlockHeader *popFromQueue(Indexes indexes);
        UsedBlockHeader *changeBlockToUsed(FreeBlockHeader *blockHeader) const;

        std::bitset<SL_SIZE> findSliInCurrentFli(Indexes indexes) const;
        Indexes findNextFreeBlocks(Indexes indexes);

        void
        createNewFreeHeader(char *blockBegin, size_t sizeOfNewBlock, bool isLast, UniversalBlock *previousPhysical);

        UsedBlockHeader *findUsedBlock(void *space);

        UsedBlockHeader *concatenateAdjacentBlocks(UsedBlockHeader *centerBlock);

        bool isMergeable(const UniversalBlock *previousBlock) const;

        void mergeTwoBlocks(UsedBlockHeader *&centerBlock, FreeBlockHeader *otherBlock);

        void removeFromQueue(const FreeBlockHeader *blockToRemove) const;
    };

    using DefaultTLSF=TLSF<>;
}

#include "TLSFImpl.hpp"

#endif //PROJECT_TLSF_HPP
