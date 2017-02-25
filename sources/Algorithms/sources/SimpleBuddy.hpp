//
// Created by dawid on 25.02.17.
//

#ifndef PROJECT_SIMPLEBUDDY_HPP
#define PROJECT_SIMPLEBUDDY_HPP

#include "Buddy.hpp"

namespace PiOS {

    /*!
     * \brief The simplest and not optimized implementation of the BuddyAlgorithm, that uses
     * binary search when searching for free page of not allocated memory
     */
    class SimpleBuddy : public Buddy {
    public:
        /*!
         * \brief Constructor. Takes information about managed space.
         * @param spaceSize size of the managed space.
         * @param spacePtr pointer to the beginning of managed space.
         * @param minRank minimum size of the page calculated as: pow(2,minRank)
         */
        SimpleBuddy(size_t spaceSize, void *spacePtr, unsigned int minRank);

        /*!
         * \brief Allocates chunk of memory.
         * @param size minimal size of space to allocate.
         * @return Information about allocated space (it's location and size).
         */
        virtual MemorySpace allocate(size_t size);

        /*!
         * \brief deallocates space, that was allocated previously.
         * @param spaceBegin beginning of the space, that is to be deallocated.
         */
        virtual void deallocate(void *spaceBegin);

        /*!
         * \brief Destructor. Deallocates all allocated memory.
         */
        virtual ~SimpleBuddy();

    private:
        size_t mOverallSpaceSize;
        void *mSpacePtr;
        unsigned int mMinRank;

        void deallocateAll();

        unsigned int findRankOfSize(size_t size);

        size_t sizeOfMemoryBlock(unsigned int rank);

        struct MemoryBlockMeta {
            MemoryBlockMeta *parent;
            MemoryBlockMeta *rightChild;
            MemoryBlockMeta *leftChild;

        };
    };

}


#endif //PROJECT_SIMPLEBUDDY_HPP
