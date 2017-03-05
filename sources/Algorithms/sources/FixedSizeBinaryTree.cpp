//
// Created by dawid on 28.02.17.
//

#include "FixedSizeBinaryTree.hpp"

namespace PiOS {

    bool NodeId::isValid() const {
        auto invalidValueRank = std::numeric_limits<decltype(mRank)>::max();
        auto invalidValueIdx = std::numeric_limits<decltype(mAbsoluteIndex)>::max();
        return mRank < invalidValueRank and mIndexInRank < invalidValueRank and mAbsoluteIndex < invalidValueIdx;
    }

    NodeId::RankType NodeId::rank() const {
        return mRank;
    }

    void NodeId::updateRank() {
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
        return mIndexInRank;
    }

    size_t NodeId::absoluteIndex() const {
        return mAbsoluteIndex;
    }

    void NodeId::updateAbsoluteIdx() {
        size_t rankOffset = calculateRankOffset();
        mAbsoluteIndex = rankOffset + mIndexInRank;
    }

    NodeId NodeId::invalidNode() {
        return NodeId(std::numeric_limits<size_t>::max());
    }


}