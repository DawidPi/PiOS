//
// Created by dawid on 1/25/18.
//

#ifndef PROJECT_LIST_HPP
#define PROJECT_LIST_HPP

#include <type_traits>
#include <iterator>

namespace PiOS {


/*!
 * Exception free API for double linked list. Allocator uses simple new operator for memory allocation.
 */
    template<typename T>
    class List {
    private:
        class Node {
        public:
            Node(T value, Node *previous = nullptr, Node *next = nullptr)
                    : mElement(value),
                      mNext(next),
                      mPrevious(previous) {}


            T &value() { return mElement; }

            const T &value() const { return mElement; }

            void setValue(const T &value) { mElement = value; }

            Node *next() const { return mNext; }

            void setNext(Node *nextNode) { mNext = nextNode; }

            Node *previous() const { return mPrevious; }

            void setPrevious(Node *previousNode) { mPrevious = previousNode; }

        private:
            T mElement;
            Node *mNext = nullptr;
            Node *mPrevious = nullptr;
        };

    public:

        class iterator {
        public:
            explicit iterator(Node *node) : mNode(node) {}

            iterator(const iterator &) = default;

            T &operator*() const { return mNode->value(); }

            iterator &operator++() { mNode = mNode->next(); }

            iterator operator++(int) {
                iterator selfCpy = *this;
                mNode = mNode->next();
                return selfCpy;
            }

            iterator &operator--() { mNode = mNode->previous(); }

            iterator operator--(int) {
                iterator selfCpy = *this;
                mNode = mNode->previous();
                return selfCpy;
            }

            bool operator==(const iterator& rhs) const {
                return mNode == rhs.mNode;
            }

            bool operator!=(const iterator& rhs) const {
                return ! (*this == rhs);
            }

        private:
            Node *mNode = nullptr;
            friend List;
        };

        class const_iterator {
        public:
            explicit const_iterator(Node *node) : mNode(node) {}

            const_iterator(const const_iterator &) = default;


            const T &operator*() { return mNode->value(); }

            const_iterator &operator++() { mNode = mNode->next(); }

            const_iterator operator++(int) {
                const_iterator selfCpy = *this;
                mNode = mNode->next();
                return selfCpy;
            }

            const_iterator &operator--() { mNode = mNode->previous(); }

            const_iterator operator--(int) {
                const_iterator selfCpy = *this;
                mNode = mNode->previous();
                return selfCpy;
            }

            bool operator==(const const_iterator& rhs) const {
                return mNode == rhs.mNode;
            }

            bool operator!=(const const_iterator& rhs) const {
                return ! (*this == rhs);
            }

        private:
            Node *mNode = nullptr;
            friend List;
        };

    public:


        List() = default;

        explicit List(std::size_t size, const T &val = T()) {
            insert(begin(), size, val);
        }

        void insert(const_iterator begin, std::size_t size, const T &val) {
            const_iterator current = begin;

            while (size-- > 0) {
                current = insert(current, val);
                ++current;
            }
        }

        const_iterator insert(const_iterator iterator, const T &val) {

            if (iterator == begin()) {
                mNodeStart = new Node(val, nullptr, nullptr);
                return begin();
            }

            auto iteratorAfter = iterator;

            const_iterator prevIterator;
            if (iterator == begin()) {

            }

            auto *node = new Node(val, iterator.mNode, iteratorAfter.mNode);
            iterator.mNode->setNext(node);


        }

        iterator begin() {
            return iterator(mNodeStart);
        }

        const_iterator begin() const {
            return const_iterator(mNodeStart);
        }

        const_iterator cbegin() const {
            return begin();
        }

        iterator end() {
            return iterator(mNodeEnd);
        }

        const_iterator end() const {
            return const_iterator(mNodeEnd);
        }

        const_iterator cend() const {
            return const_iterator(mNodeEnd);
        }

        std::size_t size() { return mSize; }

    private:
        Node *mNodeStart = nullptr;
        Node *mNodeEnd = nullptr;
        std::size_t mSize = 0;

    };
}

#endif //PROJECT_LIST_HPP
