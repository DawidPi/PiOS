//
// Created by dawid on 25.02.17.
//

#ifndef PROJECT_SIMPLEBUDDY_HPP
#define PROJECT_SIMPLEBUDDY_HPP

#include "Buddy.hpp"
#include "FixedSizeBinaryTree.hpp"
#include <limits>

namespace PiOS {

    /*!
     * \brief The simplest and not optimized implementation of the BuddyAlgorithm, that uses
     * binary search when searching for free page of not allocated memory
     */
    class SimpleBuddy : public Buddy {
    public:
        /*!
         * \brief Constructor. Takes information about managed space and binary tree implementation
         * @param binaryTree Binary tree, that encapsulates binary tree logic
         * @param spaceSize Size of the managed space.
         * @param spacePtr Pointer to the beginning of managed space.
         * @param minBlockSizeExponent Minimum size of the page calculated as: pow(2,minBlockSizeExponent)
         */
        SimpleBuddy(FixedSizeBinaryTree<size_t> &&binaryTree, size_t spaceSize, void *spacePtr,
                    unsigned int minBlockSizeExponent);

        SimpleBuddy(SimpleBuddy &&rhs);

        SimpleBuddy() = delete;
        SimpleBuddy(const SimpleBuddy& rhs) = delete;
        SimpleBuddy& operator=(const SimpleBuddy& rhs) = delete;

        /*!
         * \brief Allocates chunk of memory.
         * @param sizeToAllocate minimal size of space to allocate.
         * @return Information about allocated space (it's location and size).
         */
        MemorySpace allocate(size_t sizeToAllocate) override;

        /*!
         * \brief deallocates space, that was allocated previously.
         * @param spaceBegin beginning of the space, that is to be deallocated.
         */
        void deallocate(void *spaceBegin) override;

        /*!
         * \brief Destructor. Deallocates all allocated memory.
         */
        virtual ~SimpleBuddy();

    private:
        FixedSizeBinaryTree<size_t> mBinaryTree;
        size_t mSpaceSize;
        void *mSpacePtr;
        unsigned int mMinBlockSizeExponent;

        enum class MemoryBlockStatus : size_t {
            NOT_CREATED = std::numeric_limits<size_t>::max(),
            FULLY_ALLOCATED = 0
        };

        enum class TreePath : unsigned int {
            LEFT,
            RIGHT,
            LEFT_WITH_UPDATE,
            RIGHT_WITH_UPDATE,
            NONE
        };

        NodeId::RankType calculateOptimalRank(size_t allocate);

        size_t nodeValue(NodeId id);

        MemorySpace
        calculateMemoryPage(size_t sizeToAllocate, const NodeId &currentMemoryNode) const;

        size_t fitSizeToPage(size_t size);

        NodeId nextNode(NodeId node, size_t sizeToAllocate);

        size_t calculateMaxFreeSpaceForNode(NodeId rightChild) const;

        TreePath calculateOptimalPath(size_t leftSpace, size_t rightSpace, size_t allocationSpace);

        size_t rankToSize(NodeId::RankType rank) const;

        void *memoryNodeToPage(const NodeId &node) const;
    };

}


#endif //PROJECT_SIMPLEBUDDY_HPP
