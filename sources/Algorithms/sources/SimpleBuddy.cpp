//
// Created by dawid on 25.02.17.
//

#include "SimpleBuddy.hpp"
#include <utility>
#include <iostream>

namespace PiOS {


    SimpleBuddy::SimpleBuddy(FixedSizeBinaryTree<size_t> &&binaryTree, size_t spaceSize, void *spacePtr,
                             unsigned int minBlockSizeExponent) :
            mBinaryTree(std::forward<FixedSizeBinaryTree<size_t>>(binaryTree)),
            mSpaceSize(spaceSize),
            mSpacePtr(spacePtr),
            mMinBlockSizeExponent(minBlockSizeExponent),
            mSmallestPage(rankToSize(binaryTree.depth())) {
        mBinaryTree.initializeAllElements(static_cast<size_t>(MemoryBlockStatus::NOT_CREATED));
        mBinaryTree.setValue(mBinaryTree.root(), spaceSize);
        assert(rankToSize(mBinaryTree.depth()) > 0);
    }

    SimpleBuddy::SimpleBuddy(SimpleBuddy &&rhs) :
            SimpleBuddy(std::move(rhs.mBinaryTree), rhs.mSpaceSize, rhs.mSpacePtr, rhs.mMinBlockSizeExponent) {
    }

    MemorySpace SimpleBuddy::allocate(size_t sizeToAllocate) {
        auto currentMemoryNode = mBinaryTree.root();
        size_t currentFreeMemory = mBinaryTree.value(currentMemoryNode);

        if (sizeToAllocate > currentFreeMemory)
            return MemorySpace(nullptr, nullptr);

        auto pageSize = fitSizeToPage(sizeToAllocate);

        for (NodeId::RankType rank = 0; rank < mBinaryTree.maxTransitions() - 1; rank++) {
            NodeId newNode = nextNode(currentMemoryNode, pageSize);

            if (newNode == currentMemoryNode) {
                break;
            }
            currentMemoryNode = newNode;
        }

        MemorySpace memory = calculateMemoryToAllocate(currentMemoryNode, pageSize);
        mBinaryTree.setValue(currentMemoryNode, static_cast<size_t>(MemoryBlockStatus::FULLY_ALLOCATED));

        return memory;
    }

    MemorySpace SimpleBuddy::calculateMemoryToAllocate(const NodeId &currentMemoryNode, size_t pageSize) const {
        void* begin = memoryNodeToPage(currentMemoryNode);
        void* end = static_cast<char*>(begin) + pageSize;
        return MemorySpace(begin, end);
    }

    void SimpleBuddy::deallocate(void *) {

    }

    SimpleBuddy::~SimpleBuddy() {

    }

    size_t SimpleBuddy::fitSizeToPage(size_t size) {
        size_t currentPageSize = minPageSize();

        while (currentPageSize < size) {
            currentPageSize *= 2;
        }

        return currentPageSize;
    }

    NodeId SimpleBuddy::nextNode(NodeId node, size_t sizeToAllocate) {
        auto rightChild = mBinaryTree.rightChild(node);
        auto leftChild = mBinaryTree.leftChild(node);

        size_t maxRightFreeSpace = calculateMaxFreeSpaceForNode(rightChild);
        size_t maxLeftFreeSpace = calculateMaxFreeSpaceForNode(leftChild);


        if(!isCreated(rightChild)){
            assert(!isCreated(leftChild));
            allocateChildren(rightChild, leftChild);
        }

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

    void SimpleBuddy::allocateChildren(NodeId rightChild, NodeId leftChild){
        const auto parentValue = mBinaryTree.value(mBinaryTree.parent(rightChild));
        mBinaryTree.setValue(rightChild, parentValue / 2);
        mBinaryTree.setValue(leftChild, parentValue / 2);
    }

    size_t SimpleBuddy::calculateMaxFreeSpaceForNode(NodeId node) const {
        const bool isNodeAllocated =
                mBinaryTree.value(node) != static_cast<size_t>(MemoryBlockStatus::NOT_CREATED);
        size_t maxFreeSpace = 0;

        if (isNodeAllocated) {
            maxFreeSpace = mBinaryTree.value(node);
        } else {
            assert(node.rank() > 0);
            auto parent = mBinaryTree.parent(node);
            auto parentValue = mBinaryTree.value(parent);
            maxFreeSpace = parentValue;
        }

        return maxFreeSpace;
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
        }

        return static_cast<SimpleBuddy::TreePath>(std::numeric_limits<SimpleBuddy::TreePath>::max());
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

    size_t SimpleBuddy::minPageSize() const {
        return mSmallestPage;
    }

    bool SimpleBuddy::isCreated(NodeId node) {
        const size_t notCreatedTag = static_cast<size_t>(MemoryBlockStatus::NOT_CREATED);
        return mBinaryTree.value(node) == notCreatedTag;
    }

}