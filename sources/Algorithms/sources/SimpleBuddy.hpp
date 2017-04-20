//
// Created by dawid on 25.02.17.
//

#ifndef PROJECT_SIMPLEBUDDY_HPP
#define PROJECT_SIMPLEBUDDY_HPP

#include "FixedSizeBinaryTree.hpp"
#include "DynamicAllocator.hpp"
#include <limits>

namespace PiOS {

    /*!
     * \brief The simplest and not optimized implementation of the BuddyAlgorithm, that uses
     * binary search when searching for free page of not allocated memory
     */
    class SimpleBuddy : public DynamicAllocator {
    public:
        /*!
         * \brief Constructor. Takes information about managed space and binary tree implementation
         * \param binaryTree Binary tree, that encapsulates binary tree logic
         * \param spaceSize Size of the managed space.
         * \param spacePtr Pointer to the beginning of managed space.
         * \param minBlockSizeExponent Minimum size of the page calculated as: pow(2,minBlockSizeExponent)
         */
        explicit SimpleBuddy(FixedSizeBinaryTree<size_t> &&binaryTree, size_t spaceSize, void *spacePtr,
                             unsigned int minBlockSizeExponent);

        /*!
         * \brief Move constructor. Transfers ownership of managed resources from moved object.
         * \param rhs Object, from which ownership is transfered
         */
        SimpleBuddy(SimpleBuddy &&rhs);

        /*!
         * \brief deleted default constructor
         */
        SimpleBuddy() = delete;

        /*!
         * \brief Deleted copy constructor.
         * \param rhs
         */
        SimpleBuddy(const SimpleBuddy& rhs) = delete;

        /*!
         * \brief Deleted assignment operator.
         * \param rhs
         */
        SimpleBuddy& operator=(const SimpleBuddy& rhs) = delete;

        /*!
         * \brief Allocates chunk of memory.
         * \param sizeToAllocate minimal size of space to allocate.
         * \return Information about allocated space (it's location and size).
         */
        MemorySpace allocate(size_t sizeToAllocate) override;

        /*!
         * \brief deallocates space, that was allocated previously.
         * \param spaceBegin beginning of the space, that is to be deallocated.
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

        size_t fitSizeToPage(size_t size);

        NodeId nextNode(NodeId node, size_t sizeToAllocate);

        size_t calculateMaxFreeSpaceForNode(NodeId node) const;

        TreePath calculateOptimalPath(size_t leftSpace, size_t rightSpace, size_t allocationSpace);

        size_t rankToSize(NodeId::RankType rank) const;

        void *memoryNodeToPage(const NodeId &node) const;

        size_t minPageSize() const;

        size_t mSmallestPage;

        bool isCreated(NodeId node);

        void allocateChildren(NodeId rightChild, NodeId leftChild);

        MemorySpace calculateMemoryToAllocate(NodeId currentMemoryNode, size_t pageSize) const;

        ptrdiff_t ptrDiff(const void *ptr1, const void *ptr2);

        void destroyNode(NodeId node);

        NodeId buddy(NodeId currentNode);

        bool isNodeRoot(NodeId &currentNode) const;

        void merge(NodeId id, const NodeId secondBuddy, size_t currentPageSize);

        void markAsAllocated(NodeId currentMemoryNode);

        void findNodeToDeallocate(size_t &pageSize, NodeId &currentNode);

        bool areBuddiesUsed(size_t firstBuddyValue, size_t secondBuddyValue, size_t emptyPageSize) const;

        bool parentShouldBeUpdated(NodeId parent, const size_t biggerFreePage) const;
    };

}


#endif //PROJECT_SIMPLEBUDDY_HPP
