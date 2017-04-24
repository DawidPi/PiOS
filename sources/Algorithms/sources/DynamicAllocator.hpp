//
// Created by dapl on 2017-04-20.
//

#ifndef PROJECT_DYNAMICALLOCATOR_HPP
#define PROJECT_DYNAMICALLOCATOR_HPP


#include <cstddef>

namespace PiOS {
/*!
 * \brief Represents information about allocated space in memory
 */
    class MemorySpace {
    public:
        /*!
         * \brief constructor
         * \param begin beginning of the allocated space in memory
         * \param end the end of the allocated space in the memory, such as beginning+size=end.
         */
        explicit MemorySpace(void *begin, void *end) : mBegin(begin),
                                                       mEnd(end),
                                                       mSize(static_cast<char *>(mEnd) - static_cast<char *>(mBegin)) {}

        /*!
         * \brief gives information about beginning of the allocated memory
         * \return pointer to the beginning of the allocated memory.
         */
        void *begin() { return mBegin; }

        /*!
         * \brief gives information about end of the allocated memory, so that
         * begin() + size = end().
         * \return pointer to the byte after last allocated byte.
         */
        void *end() { return mEnd; }

        /*!
         * \brief Gives information about the size of managed memory in bytes.
         * @return Memory size in bytes.
         */
        std::size_t size() { return mSize; }

    private:
        void *mBegin;
        void *mEnd;
        std::size_t mSize;
    };


    class DynamicAllocator {
    public:
        /*!
         * \brief allocates memory
         * \param size minimum size of memory to be allocated
         * \return information about allocated memory.
         * AllocatedSpace contains null info if allocation could not be performed
         */
        virtual MemorySpace allocate(std::size_t size)=0;

        /*!
         * \brief Deallocates given space in memory. Does nothing if space is already deallocated.
         * \param spaceBegin beginning of the space to be deallocated.
         */
        virtual void deallocate(void *spaceBegin)=0;

        virtual ~DynamicAllocator() {};
    };
}


#endif //PROJECT_DYNAMICALLOCATOR_HPP
