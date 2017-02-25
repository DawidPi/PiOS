//
// Created by dawid on 25.02.17.
//

#include "SimpleBuddy.hpp"


namespace PiOS {

    SimpleBuddy::SimpleBuddy(size_t spaceSize, void *spacePtr, unsigned int minRank)
            : mOverallSpaceSize(spaceSize),
              mSpacePtr(spacePtr),
              mMinRank(minRank) {}

    MemorySpace SimpleBuddy::allocate(size_t size) {
        auto rank = findRankOfSize(size);

    }

    void SimpleBuddy::deallocate(void *spaceBegin) {

    }


    SimpleBuddy::~SimpleBuddy() {
        deallocateAll();
    }

    void SimpleBuddy::deallocateAll() {
    }

    unsigned int SimpleBuddy::findRankOfSize(size_t size) {
        unsigned int foundRank = mMinRank;

        while (size >= sizeOfMemoryBlock(foundRank))
            foundRank *= 2;

        return foundRank;
    }

    size_t SimpleBuddy::sizeOfMemoryBlock(unsigned int rank) {
        size_t memorySpace = 1;

        for (size_t i = 0; i < rank; rank++) {
            memorySpace *= 2;
        }

        return memorySpace;
    }

}