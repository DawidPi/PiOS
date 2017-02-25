//
// Created by dawid on 25.02.17.
//

#ifndef PROJECT_FIXEDSIZEBINARYTREE_HPP
#define PROJECT_FIXEDSIZEBINARYTREE_HPP

#include <array>
#include <cstdlib>
#include "power.hpp"

namespace PiOS {

    /*!
     * \brief Representation of location of the node of binary tree, to use with binary tree API.
     */
    class NodeId {
        using rankType = unsigned int;
    public:
        /*!
         * \brief Creates NodeId with given node rank and rank index in the binary tree
         * @param rank Rank(depth) of Node in the binary tree
         * @param rowIndex Index of the Node in the current rank(depth)
         */
        NodeId(unsigned int rank, unsigned int rowIndex) : mRank(rank), mRankIndex(rowIndex) {};

        /*!
         * \brief Creates NodeId with given absolute index of the node in the array.
         * @param absoluteIndex Abosulte index of node in the array.
         */
        NodeId(size_t absoluteIndex) : mAbsoluteIndex(absoluteIndex) {};

        /*!
         * \brief Allows to get rank(depth) of the node in the binary tree.
         * @return Rank(depth) of the node in binary tree.
         */
        rankType rank() const;

        /*!
         * \brief Allows to get index of the node in it's rank in binary tree.
         * @return Index of the Node in it's current rank in binary tree.
         */
        rankType rankIndex() const;

        /*!
         * \brief Allows to get absolute index of the node in the array, that @Ref FixedSizeBinaryTree uses
         * @return index of the Node in binary tree.
         */
        size_t absoluteIndex() const;

        /*!
         * \brief Checks whether Node is valid.
         * @return True if node is valid, false otherwise.
         */
        bool isValid() const;

    private:
        mutable rankType mRank = std::numeric_limits<rankType>::max();
        mutable rankType mRankIndex = std::numeric_limits<rankType>::max();
        mutable size_t mAbsoluteIndex = std::numeric_limits<size_t>::max();

        void updateRank() const;

        void updateAbsoluteIdx() const;

        size_t calculateRankOffset() const;
    };

    /*!
     * \brief Represents binary tree with fixed depth
     * @tparam T type of elements stored in the binary tree
     * @tparam depth depth of binary tree
     */
    template<typename T, unsigned int depth>
    class FixedSizeBinaryTree {
    public:
        /*!
         * \brief Creates binary tree initializing all the nodes with it's default value.
         * @param defaultValue initial value of all nodes in binary tree.
         */
        FixedSizeBinaryTree(const T &defaultValue = T());

        FixedSizeBinaryTree(const FixedSizeBinaryTree &) = delete;

        FixedSizeBinaryTree(FixedSizeBinaryTree &&) = delete;

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

    private:
        std::array<T, pow(2, depth - 1)> mContainer;

        NodeId invalidNode() const;

        size_t calculateRankOffset(unsigned int rank) const;
    };

}

#endif //PROJECT_FIXEDSIZEBINARYTREE_HPP
