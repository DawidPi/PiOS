//
// Created by dawid on 02.03.17.
//

#include "SimpleBuddyFactory.hpp"

PiOS::SimpleBuddy
PiOS::SimpleBuddyFactory::create(size_t *binaryTreeMemory, size_t binaryTreeMemoryElements, void *memory,
                                 size_t memorySize) const {
    FixedSizeBinaryTree<size_t> binaryTree(binaryTreeMemory, binaryTreeMemoryElements);
    unsigned int blockSizeExponent = calculateBlockSizeExponent(binaryTree.depth(), memorySize);
    return PiOS::SimpleBuddy{std::move(binaryTree), memorySize, memory, blockSizeExponent};
}

PiOS::NodeId::RankType
PiOS::SimpleBuddyFactory::calculateBlockSizeExponent(PiOS::FixedSizeBinaryTree<size_t>::Depth depth,
                                                     size_t size) const {
    const size_t numOfSmallestElems = pow(2, depth);
    const size_t sizeOfSmallestElem = size / numOfSmallestElems;

    unsigned int currentExponent = 0;
    size_t currentSize = 1;

    while (currentSize * 2 < sizeOfSmallestElem) {
        currentExponent++;
        currentSize *= 2;
    }

    return currentExponent;
}
