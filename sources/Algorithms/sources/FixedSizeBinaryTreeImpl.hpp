//
// Created by dawid on 25.02.17.
//

#include <limits>
#include <assert.h>

namespace PiOS {

    template<typename T>
    FixedSizeBinaryTree<T>::FixedSizeBinaryTree(T *managedSpace, size_t elementsInSpace, const T &defaultValue) :
            mManagedSpace(managedSpace),
            mElementsInSpace(elementsInSpace),
            mDepth(calculateDepth(elementsInSpace)) {
        assert(elementsInSpace > 0);

        initializeAllElements(defaultValue);
    }

    template<typename T>
    void FixedSizeBinaryTree<T>::initializeAllElements(const T &newValue) {
        size_t currentElement = 0;
        while (currentElement < mElementsInSpace) {
            new(mManagedSpace + currentElement) T(newValue);
            currentElement++;
        }
    };

    template<typename T>
    FixedSizeBinaryTree<T>::~FixedSizeBinaryTree() {
        size_t currentElement = 0;
        while (currentElement < mElementsInSpace) {
            mManagedSpace[currentElement].~T();
            currentElement++;
        }
    }

    template<typename T>
    const T &FixedSizeBinaryTree<T>::value(NodeId node) const {
        assert(isNodeValid(node));

        auto idx = node.absoluteIndex();
        return mManagedSpace[idx];
    }

    template<typename T>
    void FixedSizeBinaryTree<T>::setValue(const NodeId &node, T newValue) {
        assert(isNodeValid(node));

        auto idx = node.absoluteIndex();
        mManagedSpace[idx] = newValue;
    }

    template<typename T>
    NodeId FixedSizeBinaryTree<T>::parent(NodeId node) const {

        if (node.rank() > 0) {
            return NodeId(node.rank() - 1, node.indexInRank() / 2);
        }

        return NodeId::invalidNode();
    }

    template<typename T>
    NodeId FixedSizeBinaryTree<T>::rightChild(NodeId node) const {
        if (mDepth == 0)
            return NodeId::invalidNode();

        if (node.rank() < mDepth - 1) {
            return NodeId(node.rank() + 1, node.indexInRank() * 2 + 1);
        }

        return NodeId::invalidNode();
    };

    template<typename T>
    NodeId FixedSizeBinaryTree<T>::leftChild(NodeId node) const {
        if (mDepth == 0)
            return NodeId::invalidNode();

        if (node.rank() < mDepth - 1) {
            return NodeId(node.rank() + 1, node.indexInRank() * 2);
        }

        return NodeId::invalidNode();
    }

    template<typename T>
    size_t FixedSizeBinaryTree<T>::calculateRankOffset(unsigned int rank) const {
        size_t rankOffset = 0;
        for (unsigned int currentRank = 0; currentRank < rank; currentRank++) {
            rankOffset += pow(2, currentRank);
        }

        return rankOffset;
    }

    template<typename T>
    NodeId FixedSizeBinaryTree<T>::root() const {
        return NodeId(0);
    }

    template<typename T>
    bool FixedSizeBinaryTree<T>::isNodeValid(NodeId node) const {
        return node.isValid() and node.absoluteIndex() < mElementsInSpace;
    }

    template<typename T>
    NodeId::RankType FixedSizeBinaryTree<T>::calculateDepth(size_t size) {
        size_t currentElement = 0;
        size_t currentRankSize = 1;
        NodeId::RankType currentRank = 0;

        while (currentElement < size) {
            currentElement += currentRankSize;
            currentRankSize *= 2;
            currentRank++;
        }

        return currentRank;
    }

    template<typename T>
    FixedSizeBinaryTree<T>::FixedSizeBinaryTree(FixedSizeBinaryTree <T> &&rhs) :
            mElementsInSpace(rhs.mElementsInSpace),
            mDepth(rhs.mDepth),
            mManagedSpace(rhs.mManagedSpace) {}

    bool NodeId::isValid() const {
        unsigned int maxUnsignedInt = std::numeric_limits<unsigned int>::max();
        return (mRank < maxUnsignedInt and mIndexInRank < maxUnsignedInt) or (mAbsoluteIndex < maxUnsignedInt);
    }

    NodeId::RankType NodeId::rank() const {
        if (mRank == std::numeric_limits<RankType>::max())
            updateRank();

        return mRank;
    }

    void NodeId::updateRank() const {
        size_t currentRankSize = 1;
        RankType currentRank = 0;
        size_t currentElement = 0;

        while (mAbsoluteIndex > 2 * currentElement) {
            currentElement += currentRankSize;
            currentRankSize *= 2;
            currentRank++;
        }

        mIndexInRank = static_cast<RankType>(mAbsoluteIndex - currentElement);
        mRank = currentRank;
    }

    NodeId::RankType NodeId::indexInRank() const {
        if (mIndexInRank == std::numeric_limits<RankType>::max())
            updateRank();

        return mIndexInRank;
    }

    size_t NodeId::absoluteIndex() const {
        if (mAbsoluteIndex == std::numeric_limits<size_t>::max())
            updateAbsoluteIdx();

        return mAbsoluteIndex;
    }

    void NodeId::updateAbsoluteIdx() const {
        size_t rankOffset = calculateRankOffset();
        mAbsoluteIndex = rankOffset + mIndexInRank;
    }

    size_t NodeId::calculateRankOffset() const {
        size_t rankOffset = 0;
        for (unsigned int currentRank = 0; currentRank < mRank; currentRank++) {
            rankOffset += pow(2, currentRank);
        }

        return rankOffset;
    }

    NodeId NodeId::invalidNode() {
        return NodeId(std::numeric_limits<size_t>::max());
    }
}