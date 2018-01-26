//
// Created by dawid on 1/25/18.
//

#ifndef PROJECT_LIST_HPP
#define PROJECT_LIST_HPP

#include <type_traits>
#include <iterator>

namespace PiOS {


    namespace details {
        class Node {
        public:
            explicit Node(Node *previous = nullptr, Node *next = nullptr) :
                    mPrevious(previous),
                    mNext(next) {};

            Node(const Node &rhs) = default;

            Node *next() const { return mNext; }

            Node *previous() const { return mPrevious; }

            void setNext(Node *next) { mNext = next; }

            void setPrevious(Node *previous) { mPrevious = previous; }

        private:
            mutable Node *mPrevious;
            mutable Node *mNext;
        };
    }

/*!
 * Exception free API for double linked list. Allocator uses simple new operator for memory allocation.
 */
    template<typename T>
    class List {
    private:
        class Node : public details::Node {
        public:
            Node(T value, Node *previous = nullptr, Node *next = nullptr)
                    : details::Node(previous, next),
                      mElement(value) {}


            T &value() { return mElement; }

            const T &value() const { return mElement; }

            void setValue(const T &value) { mElement = value; }

        private:
            T mElement;
        };

    public:

        class const_iterator : public std::iterator<std::bidirectional_iterator_tag, T, long long int> {
        public:
            explicit const_iterator(const Node *node) : mNode(node) {}

            const_iterator(const const_iterator &) = default;


            const T &operator*() { return mNode->value(); }

            const_iterator &operator++() {
                mNode = static_cast<Node *>(mNode->next());
                return *this;
            }

            const_iterator operator++(int) {
                const_iterator selfCpy = *this;
                mNode = static_cast<Node *>(mNode->next());
                return selfCpy;
            }

            const_iterator &operator--() {
                mNode = static_cast<Node *>(mNode->previous());
                return *this;
            }

            const_iterator operator--(int) {
                const_iterator selfCpy = *this;
                mNode = static_cast<Node *>(mNode->previous());
                return selfCpy;
            }

            bool operator==(const const_iterator &rhs) const {
                return mNode == rhs.mNode;
            }

            bool operator!=(const const_iterator &rhs) const {
                return !(*this == rhs);
            }

        private:
            const Node *mNode = nullptr;
            friend List;
        };

        class iterator : public std::iterator<std::bidirectional_iterator_tag, T, long long int> {
        public:
            explicit iterator(Node *node) : mNode(node) {}

            iterator(const iterator &) = default;

            T &operator*() const { return mNode->value(); }

            iterator &operator++() {
                mNode = static_cast<Node *>(mNode->next());
                return *this;
            }

            iterator operator++(int) {
                iterator selfCpy = *this;
                mNode = static_cast<Node *>(mNode->next());
                return selfCpy;
            }

            iterator &operator--() {
                mNode = static_cast<Node *>(mNode->previous());
                return *this;
            }

            iterator operator--(int) {
                iterator selfCpy = *this;
                mNode = static_cast<Node *>(mNode->previous());
                return selfCpy;
            }

            bool operator==(const const_iterator &rhs) const {
                return static_cast<const_iterator>(*this) == rhs;
            }

            bool operator!=(const const_iterator &rhs) const {
                return !(*this == rhs);
            }

            operator const_iterator() const {
                return const_iterator(mNode);
            }

        private:
            Node *mNode = nullptr;
            friend List;
        };

    public:


        explicit List() :
                mNodeStart(),
                mNodeEnd() {
            mNodeStart.setNext(&mNodeEnd);
            mNodeStart.setPrevious(&mNodeEnd);
            mNodeEnd.setNext(&mNodeStart);
            mNodeEnd.setPrevious(&mNodeStart);
        }

        explicit List(std::size_t size, const T &val = T()) : List() {
            insert(begin(), size, val);
        }

        explicit List(std::initializer_list<T> init) : List() {
            for (auto &elem : init) {
                insert(end(), elem);
            }
        }

        List(const List &rhs) : List() {
            auto current = begin();
            for (auto &elem : rhs) {
                insert(current, elem);
            }
        }

        void insert(const_iterator begin, std::size_t size, const T &val) {
            const_iterator current = begin;

            while (size-- > 0) {
                insert(current, val);
            }
        }

        void insert(const_iterator cIter, const T &val) {
            auto it = iteratorCast(cIter);
            iterator previous = std::prev(it);
            auto *newNode = new(std::nothrow) Node(val, previous.mNode, it.mNode);
            previous.mNode->setNext(newNode);
            it.mNode->setPrevious(newNode);
            mSize++;
        }

        void insert(const_iterator cIter, T &&val) {
            auto it = iteratorCast(cIter);
            iterator previous = std::prev(it);
            auto *newNode = new(std::nothrow) Node(std::move(val), previous.mNode, it.mNode);
            previous.mNode->setNext(newNode);
            it.mNode->setPrevious(newNode);
            mSize++;
        }

        iterator begin() {
            return iterator(reinterpret_cast<Node *>(mNodeStart.next()));
        }

        const_iterator begin() const {
            return const_iterator(reinterpret_cast<const Node *>(mNodeStart.next()));
        }

        const_iterator cbegin() const {
            return begin();
        }

        iterator end() {
            return iterator(reinterpret_cast<Node *>(&mNodeEnd));
        }

        const_iterator end() const {
            return const_iterator(reinterpret_cast<const Node *>(&mNodeEnd));
        }

        const_iterator cend() const {
            return end();
        }

        std::size_t size() { return mSize; }

        void push_front(const T &val) {
            insert(begin(), val);
        }

        void push_front(T &&val) {
            insert(begin(), std::move(val));
        }

        void erase(const_iterator it) {
            auto toRemove = iteratorCast(it);
            auto prev = std::prev(toRemove);
            auto next = std::next(toRemove);

            delete toRemove.mNode;
            prev.mNode->setNext(next.mNode);
            next.mNode->setPrevious(prev.mNode);

            mSize--;
        }

        void erase(const_iterator begin, const_iterator end) {
            auto start = iteratorCast(begin);
            auto finish = iteratorCast(end);

            while (start != finish) {
                auto next = std::next(start);
                erase(start);
                start = next;
            }
        }

        void clear() {
            erase(begin(), end());
        }

        bool empty() {
            return mSize == 0;
        }

        template<typename... Args>
        void emplace(const_iterator it, Args &&... args) {

            auto next = iteratorCast(it);
            auto previous = std::prev(next);

            auto *node = new(std::nothrow) Node(T(std::forward<Args>(args)...), previous.mNode, next.mNode);

            previous.mNode->setNext(node);
            next.mNode->setPrevious(node);
            mSize++;
        }

    private:

        iterator iteratorCast(const_iterator it) {
            return iterator(const_cast<Node *>(it.mNode));
        }

    private:
        details::Node mNodeStart;
        details::Node mNodeEnd;
        std::size_t mSize = 0;

    };
}

#endif //PROJECT_LIST_HPP
