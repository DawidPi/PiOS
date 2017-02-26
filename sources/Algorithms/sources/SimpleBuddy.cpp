//
// Created by dawid on 25.02.17.
//

#include "SimpleBuddy.hpp"
#include <utility>

namespace PiOS {


    SimpleBuddy::SimpleBuddy(FixedSizeBinaryTree<size_t> &&binaryTree, size_t spaceSize, void *spacePtr,
                             unsigned int minBlockSizeExponent) :
            mBinaryTree(std::forward<FixedSizeBinaryTree<size_t>>(binaryTree)),
            mSpaceSize(spaceSize),
            mSpacePtr(spacePtr),
            mMinBlockSizeExponent(minBlockSizeExponent) {
        mBinaryTree.initializeAllElements(static_cast<size_t>(MemoryBlockStatus::FULLY_ALLOCATED));
        mBinaryTree.setValue(mBinaryTree.root(), spaceSize);
    }

    MemorySpace SimpleBuddy::allocate(size_t size) {
        NodeId bestNode = findOptimalNode(size);

    }

    void SimpleBuddy::deallocate(void *spaceBegin) {

    }

    SimpleBuddy::~SimpleBuddy() {

    }

    NodeId SimpleBuddy::findOptimalNode(size_t memoryToAllocate) {
        auto optimalRank = calculateOptimalRank(memoryToAllocate);
        auto currentNode = mBinaryTree.root();

        static_assert(false, "not imeplemented");


        return NodeId(0, 0);
    }

    NodeId::RankType SimpleBuddy::calculateOptimalRank(size_t memorySize) {
        NodeId::RankType foundRank = 0;
        size_t currentMemorySize = nodeValue(mBinaryTree.root());
        auto invalidRank = std::numeric_limits<NodeId::RankType>::max();
        if (currentMemorySize < memorySize) {
            return invalidRank;
        }

        while (currentMemorySize >= 2 * memorySize) {
            currentMemorySize /= 2;
            foundRank++;
        }

        return foundRank;
    }

    size_t SimpleBuddy::nodeValue(NodeId id) {
        return mBinaryTree.value(id);
    }
}