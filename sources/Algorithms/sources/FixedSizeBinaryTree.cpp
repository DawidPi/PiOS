//
// Created by dawid on 25.02.17.
//

#include <limits>
#include "FixedSizeBinaryTree.hpp"

namespace PiOS {

    template<typename T, unsigned int depth>
    FixedSizeBinaryTree<T, depth>::FixedSizeBinaryTree(const T &defaultValue) {
        for (auto &element : mContainer) {
            element = defaultValue;
        }
    };

    template<typename T, unsigned int depth>
    const T &FixedSizeBinaryTree<T, depth>::value(NodeId node) const {
        auto idx = node.absoluteIndex();
        return mContainer[idx];
    }

    template<typename T, unsigned int depth>
    void FixedSizeBinaryTree<T, depth>::setValue(const NodeId &node, T newValue) {
        auto idx = node.absoluteIndex();
        mContainer[idx] = newValue;
    }

    template<typename T, unsigned int depth>
    NodeId FixedSizeBinaryTree<T, depth>::parent(NodeId node) const {
        if (node.isValid()) {
            return NodeId(node.rank() - 1, node.rankIndex() / 2);
        }

        return invalidNode();
    }

    template<typename T, unsigned int depth>
    NodeId FixedSizeBinaryTree<T, depth>::rightChild(NodeId node) const {
        if (node.rank() < depth) {
            return NodeId(node.rank() + 1, node.rankIndex() * 2 + 1);
        }

        return invalidNode();
    };

    template<typename T, unsigned int depth>
    NodeId FixedSizeBinaryTree<T, depth>::leftChild(NodeId node) const {
        if (node.rank() < depth) {
            return NodeId(node.rank() + 1, node.rankIndex() * 2);
        }

        return invalidNode();
    }

    template<typename T, unsigned int depth>
    NodeId FixedSizeBinaryTree<T, depth>::invalidNode() const {
        unsigned int maxUnsignedInt = std::numeric_limits<unsigned int>::max();
        return NodeId(maxUnsignedInt, maxUnsignedInt);
    }

    template<typename T, unsigned int depth>
    size_t FixedSizeBinaryTree<T, depth>::calculateRankOffset(unsigned int rank) const {
        size_t rankOffset = 0;
        for (unsigned int currentRank = 0; currentRank < rank; currentRank++) {
            rankOffset += pow(2, currentRank);
        }

        return rankOffset;
    }


    bool NodeId::isValid() const {
        unsigned int maxUnsignedInt = std::numeric_limits<unsigned int>::max();
        return mRank < maxUnsignedInt and mRankIndex < maxUnsignedInt;
    }

    NodeId::rankType NodeId::rank() const {
        if (mRank == std::numeric_limits<rankType>::max())
            updateRank();

        return mRank;
    }

    void NodeId::updateRank() const {
        size_t currentElement = 1;
        rankType currentRank = 0;

        while (mAbsoluteIndex > currentElement) {
            currentElement *= 2;
            currentRank++;
        }

        mRankIndex = static_cast<rankType>(mAbsoluteIndex - currentElement);
        mRank = currentRank;
    }

    NodeId::rankType NodeId::rankIndex() const {
        if (mRankIndex == std::numeric_limits<rankType>::max())
            updateRank();

        return mRank;
    }

    size_t NodeId::absoluteIndex() const {
        if (mAbsoluteIndex == std::numeric_limits<size_t>::max())
            updateAbsoluteIdx();

        return mAbsoluteIndex;
    }

    void NodeId::updateAbsoluteIdx() const {
        size_t rankOffset = calculateRankOffset();
        mAbsoluteIndex = rankOffset + mRankIndex;
    }

    size_t NodeId::calculateRankOffset() const {
        size_t rankOffset = 0;
        for (unsigned int currentRank = 0; currentRank < mRank; currentRank++) {
            rankOffset += pow(2, currentRank);
        }

        return rankOffset;
    }
}