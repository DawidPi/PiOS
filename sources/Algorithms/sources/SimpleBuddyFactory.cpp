//
// Created by dawid on 02.03.17.
//

#include "SimpleBuddyFactory.hpp"

PiOS::SimpleBuddy
PiOS::SimpleBuddyFactory::create(std::size_t *binaryTreeMemory, std::size_t binaryTreeMemoryElements, void *memory,
                                 std::size_t memorySize) const {
    FixedSizeBinaryTree<std::size_t> binaryTree(binaryTreeMemory, binaryTreeMemoryElements);
    unsigned int blockSizeExponent = calculateBlockSizeExponent(binaryTree.depth(), memorySize);
    return PiOS::SimpleBuddy{std::move(binaryTree), memorySize, memory, blockSizeExponent};
}

PiOS::NodeId::RankType
PiOS::SimpleBuddyFactory::calculateBlockSizeExponent(PiOS::FixedSizeBinaryTree<std::size_t>::Depth depth,
                                                     std::size_t size) const {
    const std::size_t numOfSmallestElems = pow(2, depth);
    const std::size_t sizeOfSmallestElem = size / numOfSmallestElems;

    unsigned int currentExponent = 0;
    std::size_t currentSize = 1;

    while (currentSize * 2 < sizeOfSmallestElem) {
        currentExponent++;
        currentSize *= 2;
    }

    return currentExponent;
}
