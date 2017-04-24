//
// Created by dawid on 25.02.17.
//

#include "SimpleBuddy.hpp"
#include <iostream>

namespace PiOS {


    SimpleBuddy::SimpleBuddy(FixedSizeBinaryTree<std::size_t> &&binaryTree, std::size_t spaceSize, void *spacePtr,
                             unsigned int minBlockSizeExponent) :
            mBinaryTree(std::forward<FixedSizeBinaryTree<std::size_t>>(binaryTree)),
            mSpaceSize(spaceSize),
            mSpacePtr(spacePtr),
            mMinBlockSizeExponent(minBlockSizeExponent),
            mSmallestPage(rankToSize(mBinaryTree.maxTransitions())) {
        mBinaryTree.initializeAllElements(static_cast<std::size_t>(MemoryBlockStatus::NOT_CREATED));
        mBinaryTree.setValue(mBinaryTree.root(), spaceSize);
        assert(minPageSize() > 0);
    }

    SimpleBuddy::SimpleBuddy(SimpleBuddy &&rhs) :
            SimpleBuddy(std::move(rhs.mBinaryTree), rhs.mSpaceSize, rhs.mSpacePtr, rhs.mMinBlockSizeExponent) {
    }

    MemorySpace SimpleBuddy::allocate(std::size_t sizeToAllocate) {
        auto currentMemoryNode = mBinaryTree.root();
        std::size_t currentTotalFreeMemory = mBinaryTree.value(currentMemoryNode);

        if (sizeToAllocate > currentTotalFreeMemory)
            return MemorySpace(nullptr, nullptr);

        const auto pageSize = fitSizeToPage(sizeToAllocate);

        for (NodeId::RankType rank = 0; rank < mBinaryTree.maxTransitions(); rank++) {
            NodeId next = nextNode(currentMemoryNode, pageSize);

            const bool treeTraversalFinished = next == currentMemoryNode;
            if (treeTraversalFinished)
                break;

            currentMemoryNode = next;
        }

        MemorySpace memory = calculateMemoryToAllocate(currentMemoryNode, pageSize);
        markAsAllocated(currentMemoryNode);

        return memory;
    }

    void SimpleBuddy::markAsAllocated(NodeId currentMemoryNode) {
        mBinaryTree.setValue(currentMemoryNode, static_cast<std::size_t>(MemoryBlockStatus::FULLY_ALLOCATED));
    }

    MemorySpace SimpleBuddy::calculateMemoryToAllocate(NodeId currentMemoryNode, std::size_t pageSize) const {
        void *begin = memoryNodeToPage(currentMemoryNode);
        void *end = static_cast<char *>(begin) + pageSize;
        return MemorySpace(begin, end);
    }

    void SimpleBuddy::deallocate(void *deallocationSpaceStart) {
        if (deallocationSpaceStart == nullptr)
            return;

        auto pageSize = minPageSize();
        const NodeId::RankType startRank = mBinaryTree.depth() - 1;
        const auto startOffsetInRank = static_cast<unsigned int>(ptrDiff(deallocationSpaceStart, mSpacePtr) / pageSize);

        NodeId currentNode(startRank, startOffsetInRank);

        findNodeToDeallocate(pageSize, currentNode);
        mBinaryTree.setValue(currentNode, pageSize);

        while (not isNodeRoot(currentNode)) {
            const auto buddyNode = buddy(currentNode);
            assert(isCreated(buddyNode));

            const auto parent = mBinaryTree.parent(currentNode);
            const auto buddyValue = mBinaryTree.value(buddyNode);
            const auto currentValue = mBinaryTree.value(currentNode);

            if (not areBuddiesUsed(currentValue, buddyValue, pageSize)) {
                merge(currentNode, buddyNode, pageSize);
            } else {
                const auto biggerFreePage = std::max(currentValue, buddyValue);

                if (parentShouldBeUpdated(parent, biggerFreePage))
                    mBinaryTree.setValue(parent, biggerFreePage);
                else
                    break;
            }

            pageSize *= 2;
            currentNode = parent;
        }
    }

    bool SimpleBuddy::parentShouldBeUpdated(NodeId parent, const std::size_t biggerFreePage) const {
        return mBinaryTree.value(parent) != biggerFreePage;
    }

    bool SimpleBuddy::areBuddiesUsed(std::size_t firstBuddyValue, std::size_t secondBuddyValue,
                                     std::size_t emptyPageSize) const {
        return firstBuddyValue != emptyPageSize or secondBuddyValue != emptyPageSize;
    }

    void SimpleBuddy::findNodeToDeallocate(std::size_t &pageSize, NodeId &currentNode) {
        while (!isCreated(currentNode)) {
            currentNode = mBinaryTree.parent(currentNode);
            pageSize *= 2;
        }
    }

    bool SimpleBuddy::isNodeRoot(NodeId &currentNode) const { return currentNode == mBinaryTree.root(); }

    SimpleBuddy::~SimpleBuddy() {

    }

    std::size_t SimpleBuddy::fitSizeToPage(std::size_t size) {
        std::size_t currentPageSize = minPageSize();

        while (currentPageSize < size) {
            currentPageSize *= 2;
        }

        return currentPageSize;
    }

    NodeId SimpleBuddy::nextNode(NodeId node, std::size_t sizeToAllocate) {
        auto rightChild = mBinaryTree.rightChild(node);
        auto leftChild = mBinaryTree.leftChild(node);

        std::size_t maxRightFreeSpace = calculateMaxFreeSpaceForNode(rightChild);
        std::size_t maxLeftFreeSpace = calculateMaxFreeSpaceForNode(leftChild);

        auto currentValue = mBinaryTree.value(node);

        if (!isCreated(rightChild) and currentValue > sizeToAllocate) {
            assert(!isCreated(leftChild)); // children are created and destroyed in pairs


            allocateChildren(rightChild, leftChild);

            auto parentSize = mBinaryTree.value(node);
            mBinaryTree.setValue(node, parentSize / 2);
        } else if (!isCreated(rightChild) and currentValue == sizeToAllocate) {
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
                std::size_t freeMemorySpaceInRightNode = maxRightFreeSpace - sizeToAllocate;
                std::size_t newNodeMemoryValue = std::max(maxLeftFreeSpace, freeMemorySpaceInRightNode);
                mBinaryTree.setValue(node, newNodeMemoryValue);
            }
                break;
            case TreePath::LEFT_WITH_UPDATE: {
                nextNode = leftChild;
                std::size_t freeMemorySpaceInLeftNode = maxLeftFreeSpace - sizeToAllocate;
                std::size_t newNodeMemoryValue = std::max(maxRightFreeSpace, freeMemorySpaceInLeftNode);
                mBinaryTree.setValue(node, newNodeMemoryValue);
            }
                break;
            case TreePath::NONE:
                break;
        }

        return nextNode;
    }

    void SimpleBuddy::allocateChildren(NodeId rightChild, NodeId leftChild) {
        const auto parentValue = mBinaryTree.value(mBinaryTree.parent(rightChild));
        mBinaryTree.setValue(rightChild, parentValue / 2);
        mBinaryTree.setValue(leftChild, parentValue / 2);
    }

    std::size_t SimpleBuddy::calculateMaxFreeSpaceForNode(NodeId node) const {
        const bool isNodeAllocated =
                mBinaryTree.value(node) != static_cast<std::size_t>(MemoryBlockStatus::NOT_CREATED);
        std::size_t maxFreeSpace = 0;

        if (isNodeAllocated) {
            maxFreeSpace = mBinaryTree.value(node);
        } else {
            assert(node.rank() > 0);
            auto parent = mBinaryTree.parent(node);
            auto parentValue = mBinaryTree.value(parent);
            maxFreeSpace = parentValue / 2;
        }

        return maxFreeSpace;
    }

    SimpleBuddy::TreePath
    SimpleBuddy::calculateOptimalPath(std::size_t leftSpace, std::size_t rightSpace, std::size_t allocationSpace) {

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
        return static_cast<SimpleBuddy::TreePath>(std::numeric_limits<std::size_t>::max());
    }

    std::size_t SimpleBuddy::rankToSize(NodeId::RankType rank) const {
        std::size_t calculatedSize = mSpaceSize;
        while (rank != 0) {
            calculatedSize /= 2;
            rank--;
        }

        return calculatedSize;
    }

    void *SimpleBuddy::memoryNodeToPage(const NodeId &node) const {
        std::size_t rankOffsets = rankToSize(node.rank());
        return reinterpret_cast<char *>(mSpacePtr) + node.indexInRank() * rankOffsets;
    }

    std::size_t SimpleBuddy::minPageSize() const {
        return mSmallestPage;
    }

    bool SimpleBuddy::isCreated(NodeId node) {
        const std::size_t notCreatedTag = static_cast<std::size_t>(MemoryBlockStatus::NOT_CREATED);
        return mBinaryTree.value(node) != notCreatedTag;
    }

    ptrdiff_t SimpleBuddy::ptrDiff(const void *ptr1, const void *ptr2) {
        return static_cast<const char *>(ptr1) - static_cast<const char *>(ptr2);
    }

    void SimpleBuddy::destroyNode(NodeId node) {
        mBinaryTree.setValue(node, static_cast<std::size_t>(MemoryBlockStatus::NOT_CREATED));
    }

    NodeId SimpleBuddy::buddy(NodeId currentNode) {
        auto isCurrentRightBuddy = static_cast<bool>(currentNode.indexInRank() % 2);
        if (isCurrentRightBuddy) {
            return NodeId(currentNode.rank(), currentNode.indexInRank() - 1);
        } else {
            return NodeId(currentNode.rank(), currentNode.indexInRank() + 1);
        }
    }

    void SimpleBuddy::merge(NodeId firstBuddy, const NodeId secondBuddy, std::size_t currentPageSize) {
        destroyNode(firstBuddy);
        destroyNode(secondBuddy);
        mBinaryTree.setValue(mBinaryTree.parent(firstBuddy), currentPageSize * 2);
    }

}