//
// Created by dawid on 25.02.17.
//

#ifndef PROJECT_FIXEDSIZEBINARYTREE_HPP
#define PROJECT_FIXEDSIZEBINARYTREE_HPP

#include <array>
#include <cstdlib>
#include <limits>
#include "power.hpp"

namespace PiOS {

    /*!
     * \brief Representation of location of the node of binary tree, to use with binary tree API.
     */
    class NodeId {
    public:
        /*!
         *
         * \brief New type for describing rank in BinaryTree
         */
        using RankType = unsigned int;
        /*!
         * \brief Creates NodeId with given node rank and rank index in the binary tree
         * @param rank Rank(depth) of Node in the binary tree
         * @param rowIndex Index of the Node in the current rank(depth)
         */
        explicit NodeId(unsigned int rank, unsigned int rowIndex) : mRank(rank), mIndexInRank(rowIndex) {
            auto invalidValue = std::numeric_limits<decltype(rank)>::max();
            if (rank == invalidValue or rowIndex == invalidValue)
                return;

            updateAbsoluteIdx();
        };

        /*!
         * \brief Creates NodeId with given absolute index of the node in the array.
         * @param absoluteIndex Abosulte index of node in the array.
         */
        explicit NodeId(size_t absoluteIndex) : mAbsoluteIndex(absoluteIndex) {
            auto invalidValue = std::numeric_limits<decltype(absoluteIndex)>::max();
            if (absoluteIndex == invalidValue)
                return;
            updateRank();
        };

        /*!
         * \brief Comparison operator.
         * @param rhs Right hand side of the comparison.
         * @return true if both nodes represent the same value.
         */
        bool operator==(const NodeId &rhs) const {
            return absoluteIndex() == rhs.absoluteIndex();
        }

        /*!
         * \brief Allows to get rank(depth) of the node in the binary tree.
         * @return Rank(depth) of the node in binary tree.
         */
        RankType rank() const;

        /*!
         * \brief Allows to get index of the node in it's rank in binary tree.
         * @return Index of the Node in it's current rank in binary tree.
         */
        RankType indexInRank() const;

        /*!
         * \brief Allows to get absolute index of the node in the array, that @ref FixedSizeBinaryTree uses
         * @return index of the Node in binary tree.
         */
        size_t absoluteIndex() const;

        /*!
         * \brief Checks whether Node is valid.
         * @return True if node is valid, false otherwise.
         */
        bool isValid() const;

        /*!
         * Creates invalid Node, which does not represent a proper entry in binaryTree
         * @return invalid Node.
         */
        static NodeId invalidNode();

    private:
        RankType mRank = std::numeric_limits<RankType>::max();
        RankType mIndexInRank = std::numeric_limits<RankType>::max();
        size_t mAbsoluteIndex = std::numeric_limits<size_t>::max();

        void updateRank();

        void updateAbsoluteIdx();
        size_t calculateRankOffset() const;
    };

    /*!
     * \brief Represents binary tree with fixed depth
     * @tparam T type of elements stored in the binary tree
     * @tparam depth depth of binary tree
     */
    template<typename T>
    class FixedSizeBinaryTree {
    public:
        /*!
         * \brief new type, where depth of the tree can be safely stored.
         */
        using Depth = unsigned int;

        /*!
         * \brief Creates binary tree initializing all the nodes with it's default value.
         * @param managedSpace Pointer to the continous memory block, that is to be managed
         * @param elementsInSpace Length of memory block pointed by  managedSpace
         * @param defaultValue Initial value of all nodes in binary tree.
         */
        explicit FixedSizeBinaryTree(T *managedSpace, size_t elementsInSpace, const T &defaultValue = T());

        /*!
         * \brief Deleted copy c'tor, because 2 objects cannot manage same resource
         */
        FixedSizeBinaryTree(const FixedSizeBinaryTree<T> &) = delete;

        /*!
         * \brief Deleted assignment operator, because 2 objects cannot manage same resource
         */
        FixedSizeBinaryTree& operator=(const FixedSizeBinaryTree<T> &) = delete;

        /*!
         * \brief Move constructor. transfers all knowledge from rhs to this object.
         * @param rhs FixedSizeBinaryTree to be moved
         */
        explicit FixedSizeBinaryTree(FixedSizeBinaryTree &&rhs);

        /*!
         * \brief Allows for an access to the value of the Node
         * @param node Node, which value is to be returned
         * @return Value contained in the node.
         */
        const T &value(NodeId node) const;

        /*!
         * \brief Sets new value of given node.
         * @param node Node, which value is to be set.
         * @param newValue New value contained by the node.
         */
        void setValue(const NodeId &node, T newValue);

        /*!
         * \brief Gets parent node of the given node
         * @param node Node, which parent is to be returned
         * @return Parent of the given node. If given node is a root, then
         * invalid node is returned.
         */
        NodeId parent(NodeId node) const;

        /*!
         * \brief Gets right child of the given Node.
         * @param node Node, which right child is to be returned.
         * @return Right child of the given node. If child could not be fetched
         * (final leaf given as parameter), then invalid node is returned.
         */
        NodeId rightChild(NodeId node) const;

        /*!
         * \brief Gets left child of the given Node.
         * @param node Node, which left child is to be returned.
         * @return Left child of the given node. If child could not be fetched
         * (final leaf given as parameter), then invalid node is returned.
         */
        NodeId leftChild(NodeId node) const;

        /*!
         * Allows to get proxy to the root of the binary tree.
         * @return Root of the binary tree.
         */
        NodeId root() const;

        /*!
         * changes values of all nodes to the value of given argument
         * @param newValue value to which all nodes' values will be changed
         */
        void initializeAllElements(const T &newValue);

        /*!
         * \brief Returns depth of the tree counting from 1.
         * @return Depth of the tree.
         */
        Depth depth() const { return mDepth; }

        /*!
         * \brief Helper method, when trying to iterate over transitions not ranks
         * @return Max number of transitions counting from root.
         */
        Depth maxTransitions() const { return depth() - 1; }

        /*!
         * \brief destructor. Calls approriate destructors of managed elements
         */
        ~FixedSizeBinaryTree();
    private:
        Depth mDepth;
        T *const mManagedSpace;
        size_t mElementsInSpace;

        bool isNodeValid(NodeId node) const;
        size_t calculateRankOffset(unsigned int rank) const;

        NodeId::RankType calculateDepth(size_t size);
    };

}

#include "FixedSizeBinaryTreeImpl.hpp"

#endif //PROJECT_FIXEDSIZEBINARYTREE_HPP
