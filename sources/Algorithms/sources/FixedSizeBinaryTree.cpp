//
// Created by dawid on 28.02.17.
//

#include "FixedSizeBinaryTree.hpp"

namespace PiOS {

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

    size_t NodeId::calculateRankOffset() const {
        size_t rankOffset = 0;
        for (unsigned int currentRank = 0; currentRank < mRank; currentRank++) {
            rankOffset += pow(2, currentRank);
        }

        return rankOffset;
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

    NodeId NodeId::invalidNode() {
        return NodeId(std::numeric_limits<size_t>::max());
    }


}