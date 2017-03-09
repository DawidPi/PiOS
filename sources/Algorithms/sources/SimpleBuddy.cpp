//
// Created by dawid on 25.02.17.
//

#include "SimpleBuddy.hpp"
#include <iostream>

namespace PiOS {


    SimpleBuddy::SimpleBuddy(FixedSizeBinaryTree<size_t> &&binaryTree, size_t spaceSize, void *spacePtr,
                             unsigned int minBlockSizeExponent) :
            mBinaryTree(std::forward<FixedSizeBinaryTree<size_t>>(binaryTree)),
            mSpaceSize(spaceSize),
            mSpacePtr(spacePtr),
            mMinBlockSizeExponent(minBlockSizeExponent),
            mSmallestPage(rankToSize(mBinaryTree.maxTransitions())) {
        mBinaryTree.initializeAllElements(static_cast<size_t>(MemoryBlockStatus::NOT_CREATED));
        mBinaryTree.setValue(mBinaryTree.root(), spaceSize);
        assert(minPageSize() > 0);
    }

    SimpleBuddy::SimpleBuddy(SimpleBuddy &&rhs) :
            SimpleBuddy(std::move(rhs.mBinaryTree), rhs.mSpaceSize, rhs.mSpacePtr, rhs.mMinBlockSizeExponent) {
    }

    MemorySpace SimpleBuddy::allocate(size_t sizeToAllocate) {
        auto currentMemoryNode = mBinaryTree.root();
        size_t currentTotalFreeMemory = mBinaryTree.value(currentMemoryNode);

        if (sizeToAllocate > currentTotalFreeMemory)
            return MemorySpace(nullptr, nullptr);

        auto pageSize = fitSizeToPage(sizeToAllocate);

        for (NodeId::RankType rank = 0; rank < mBinaryTree.maxTransitions(); rank++) {
            NodeId next = nextNode(currentMemoryNode, pageSize);

            if (next == currentMemoryNode) {
                break;
            }
            currentMemoryNode = next;
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

    void SimpleBuddy::deallocate(void *deallocationSpaceStart) {
        if (deallocationSpaceStart == nullptr)
            return;

        auto pageSize = minPageSize();
        const NodeId::RankType startRank = mBinaryTree.depth() - 1;
        const std::ptrdiff_t startOffsetInRank = static_cast<decltype(startOffsetInRank)>(
                                                         ptrDiff(deallocationSpaceStart, mSpacePtr)) / pageSize;

        NodeId currentNode(startRank, startOffsetInRank);

        while (!isCreated(currentNode)) {
            currentNode = mBinaryTree.parent(currentNode);
            pageSize *= 2;
        }

        mBinaryTree.setValue(currentNode, pageSize);

        while (not isNodeRoot(currentNode)) {
            const auto buddyNode = buddy(currentNode);
            assert(isCreated(buddyNode));
            const auto parent = mBinaryTree.parent(currentNode);
            const size_t buddyValue = mBinaryTree.value(buddyNode);
            const auto currentValue = mBinaryTree.value(currentNode);

            const bool isBuddyUsed = buddyValue != pageSize;
            const bool isCurrentUsed = currentValue != pageSize;

            if (not isBuddyUsed and not isCurrentUsed) {
                merge(currentNode, buddyNode, pageSize);
            } else {

                const auto biggerFreePage = std::max(currentValue, buddyValue);

                if (mBinaryTree.value(parent) != biggerFreePage)
                    mBinaryTree.setValue(parent, biggerFreePage);
                else
                    break;
            }

            pageSize *= 2;
            currentNode = parent;
        }
    }

    bool SimpleBuddy::isNodeRoot(NodeId &currentNode) const { return currentNode == mBinaryTree.root(); }

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

        auto currentValue = mBinaryTree.value(node);

        if(!isCreated(rightChild) and currentValue > sizeToAllocate){
            assert(!isCreated(leftChild)); // children are created and destroyed in pairs


            allocateChildren(rightChild, leftChild);

            auto parentSize = mBinaryTree.value(node);
            mBinaryTree.setValue(node, parentSize/2);
        } else if (!isCreated(rightChild) and currentValue == sizeToAllocate){
            return node;
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
            case TreePath::RIGHT_WITH_UPDATE: {
                nextNode = rightChild;
                size_t freeMemorySpaceInRightNode = maxRightFreeSpace - sizeToAllocate;
                size_t newNodeMemoryValue = std::max(maxLeftFreeSpace, freeMemorySpaceInRightNode);
                mBinaryTree.setValue(node, newNodeMemoryValue);
            }
                break;
            case TreePath::LEFT_WITH_UPDATE: {
                nextNode = leftChild;
                size_t freeMemorySpaceInLeftNode = maxLeftFreeSpace - sizeToAllocate;
                size_t newNodeMemoryValue = std::max(maxRightFreeSpace, freeMemorySpaceInLeftNode);
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
            maxFreeSpace = parentValue/2;
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

        assert(false);
        return static_cast<SimpleBuddy::TreePath>(std::numeric_limits<size_t>::max());
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
        return reinterpret_cast<char*>(mSpacePtr) + node.indexInRank() * rankOffsets;
    }

    size_t SimpleBuddy::minPageSize() const {
        return mSmallestPage;
    }

    bool SimpleBuddy::isCreated(NodeId node) {
        const size_t notCreatedTag = static_cast<size_t>(MemoryBlockStatus::NOT_CREATED);
        return mBinaryTree.value(node) != notCreatedTag;
    }

    ptrdiff_t SimpleBuddy::ptrDiff(const void *ptr1, const void *ptr2) {
        return static_cast<const char *>(ptr1) - static_cast<const char *>(ptr2);
    }

    void SimpleBuddy::destroyNode(NodeId node) {
        mBinaryTree.setValue(node, static_cast<size_t>(MemoryBlockStatus::NOT_CREATED));
    }

    NodeId SimpleBuddy::buddy(NodeId currentNode) {
        auto isCurrentRightBuddy = static_cast<bool>(currentNode.indexInRank() % 2);
        if (isCurrentRightBuddy) {
            return NodeId(currentNode.rank(), currentNode.indexInRank() - 1);
        } else {
            return NodeId(currentNode.rank(), currentNode.indexInRank() + 1);
        }
    }

    void SimpleBuddy::merge(NodeId firstBuddy, const NodeId secondBuddy, size_t currentPageSize) {
        destroyNode(firstBuddy);
        destroyNode(secondBuddy);
        mBinaryTree.setValue(mBinaryTree.parent(firstBuddy), currentPageSize * 2);
    }

}