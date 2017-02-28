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

    MemorySpace SimpleBuddy::allocate(size_t sizeToAllocate) {
        NodeId currentMemoryNode = mBinaryTree.root();
        size_t currentFreeMemory = mBinaryTree.value(currentMemoryNode);
        NodeId leftChildNode = mBinaryTree.leftChild(currentMemoryNode);
        NodeId rightChildNode = mBinaryTree.rightChild(currentMemoryNode);
        NodeId::RankType rank = 0;

        if (sizeToAllocate > currentFreeMemory)
            return MemorySpace(nullptr, nullptr);

        auto pageSize = fitSizeToPage(sizeToAllocate);

        while (rank < mBinaryTree.depth()) {
            rank++;

            NodeId newNode = nextNode(currentMemoryNode, pageSize);

            if (newNode == currentMemoryNode) {
                break;
            }
        }

        mBinaryTree.setValue(currentMemoryNode, static_cast<size_t>(MemoryBlockStatus::FULLY_ALLOCATED));
        MemorySpace memorySpace = calculateMemoryPage(sizeToAllocate, currentMemoryNode);

        return memorySpace;
    }

    MemorySpace SimpleBuddy::calculateMemoryPage(size_t sizeToAllocate, const NodeId &currentMemoryNode) const {

        void *memoryBegin = nullptr;
        void *memoryEnd = nullptr;
        size_t currentFreeMemory = mBinaryTree.value(currentMemoryNode);
        if (currentFreeMemory >= sizeToAllocate) {
            memoryBegin = memoryNodeToPage(currentMemoryNode);
            memoryEnd = reinterpret_cast<void *>(reinterpret_cast<size_t>(memoryBegin) + currentFreeMemory);
        }

        MemorySpace memorySpace(memoryBegin, memoryEnd);
        return memorySpace;
    }

    void SimpleBuddy::deallocate(void *spaceBegin) {

    }

    SimpleBuddy::~SimpleBuddy() {

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

    size_t SimpleBuddy::fitSizeToPage(size_t size) {
        const NodeId::RankType startRank = 0;
        size_t currentPageSize = rankToSize(startRank);

        while (size > currentPageSize) {
            currentPageSize *= 2;
        }

        return currentPageSize;
    }

    NodeId SimpleBuddy::nextNode(NodeId node, size_t sizeToAllocate) {
        auto rightChild = mBinaryTree.rightChild(node);
        auto leftChild = mBinaryTree.leftChild(node);

        size_t maxRightFreeSpace = calculateMaxFreeSpaceForNode(rightChild);
        size_t maxLeftFreeSpace = calculateMaxFreeSpaceForNode(leftChild);

        TreePath path = calculateOptimalPath(maxLeftFreeSpace, maxRightFreeSpace, sizeToAllocate);

        NodeId nextNode = node;

        switch (path) {
            case TreePath::LEFT:
                nextNode = leftChild;
                break;
            case TreePath::RIGHT:
                nextNode = rightChild;
                break;
            case TreePath::LEFT_WITH_UPDATE: {
                nextNode = rightChild;
                size_t freeMemorySpaceInRightNode = maxRightFreeSpace - sizeToAllocate;
                size_t newNodeMemoryValue = std::min(maxLeftFreeSpace, freeMemorySpaceInRightNode);
                mBinaryTree.setValue(node, newNodeMemoryValue);
            }
                break;
            case TreePath::RIGHT_WITH_UPDATE: {
                nextNode = leftChild;
                size_t freeMemorySpaceInLeftNode = maxLeftFreeSpace - sizeToAllocate;
                size_t newNodeMemoryValue = std::min(maxRightFreeSpace, freeMemorySpaceInLeftNode);
                mBinaryTree.setValue(node, newNodeMemoryValue);
            }
                break;
            case TreePath::NONE:
                break;
        }

        return nextNode;
    }

    size_t SimpleBuddy::calculateMaxFreeSpaceForNode(NodeId rightChild) const {
        const bool isRightChildAllocated =
                mBinaryTree.value(rightChild) != static_cast<size_t>(MemoryBlockStatus::NOT_CREATED);
        size_t maxRightFreeSpace = 0;
        if (isRightChildAllocated) {
            maxRightFreeSpace = mBinaryTree.value(rightChild);
        } else {
            maxRightFreeSpace = rankToSize(rightChild.rank());
        }

        return maxRightFreeSpace;
    }

    SimpleBuddy::TreePath
    SimpleBuddy::calculateOptimalPath(size_t leftSpace, size_t rightSpace, size_t allocationSpace) {
        if (leftSpace < rightSpace and leftSpace >= allocationSpace) {
            return TreePath::LEFT;
        } else if (rightSpace < leftSpace and rightSpace >= allocationSpace) {
            return TreePath::RIGHT;
        } else if (leftSpace < rightSpace and leftSpace < allocationSpace and rightSpace >= allocationSpace) {
            return TreePath::RIGHT_WITH_UPDATE;
        } else if (rightSpace < leftSpace and rightSpace < allocationSpace and leftSpace >= allocationSpace) {
            return TreePath::LEFT_WITH_UPDATE;
        } else if (rightSpace == leftSpace && leftSpace >= allocationSpace) {
            return TreePath::LEFT;
        } else if (leftSpace < allocationSpace and rightSpace < allocationSpace) {
            return TreePath::NONE;
        } else {
            assert(false);
        }
    }

    size_t SimpleBuddy::rankToSize(NodeId::RankType rank) const {
        size_t calculatedSize = mSpaceSize;
        while (rank != 0) {
            calculatedSize /= 2;
            rank--;
        }

        return calculatedSize;
    }

    void *SimpleBuddy::memoryNodeToPage(const NodeId &node) const {
        size_t rankOffsets = rankToSize(node.rank());
        return reinterpret_cast<void *>(reinterpret_cast<size_t>(mSpacePtr) + node.indexInRank() * rankOffsets);
    }

}