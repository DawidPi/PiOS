//
// Created by dawid on 02.03.17.
//

#ifndef PROJECT_SIMPLEBUDDYFACTORY_HPP
#define PROJECT_SIMPLEBUDDYFACTORY_HPP

#include "SimpleBuddy.hpp"

namespace PiOS {

    class SimpleBuddyFactory {
    public:
        PiOS::SimpleBuddy create(size_t *binaryTreeMemory, size_t binaryTreeMemoryElements, void *memory,
                                 size_t memorySize) const;

    private:
        PiOS::NodeId::RankType
        calculateBlockSizeExponent(PiOS::FixedSizeBinaryTree<size_t>::Depth depth, size_t size) const;
    };

}


#endif //PROJECT_SIMPLEBUDDYFACTORY_HPP
