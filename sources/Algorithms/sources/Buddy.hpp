//
// Created by dawid on 24.02.17.
//

#ifndef PROJECT_BUDDY_HPP
#define PROJECT_BUDDY_HPP

#include <cstdlib>

namespace PiOS {

    /*!
     * \brief Represents information about allocated space in memory
     */
    class MemorySpace {
    public:
        /*!
         * \brief constructor
         * @param begin beginning of the allocated space in memory
         * @param end the end of the allocated space in the memory, such as beginning+size=end.
         */
        MemorySpace(void *begin, void *end) : mBegin(begin), mEnd(end) {}

        /*!
         * \brief gives information about beginning of the allocated memory
         * @return pointer to the beginning of the allocated memory.
         */
        void *begin() { return mBegin; }

        /*!
         * \brief gives information about end of the allocated memory, so that
         * begin() + size = end().
         * @return pointer to the byte after last allocated byte.
         */
        void *end() { return mEnd; }

    private:
        void *mBegin;
        void *mEnd;
    };


    /*!
     *  \brief Abstract class for different Buddy Algorithms implementation.
     */
    class Buddy {
    public:
        /*!
         * \brief allocates memory
         * @param size minimum size of memory to be allocated
         * @return information about allocated memory.
         * AllocatedSpace contains null info if allocation could not be performed
         */
        virtual MemorySpace allocate(size_t size)=0;

        /*!
         * \brief Deallocates given space in memory. Does nothing if space is already deallocated.
         * @param spaceBegin beginning of the space to be deallocated.
         */
        virtual void deallocate(void *spaceBegin)=0;

        /*!
         * \brief Destructor. Deallocates all allocated memory.
         */
        virtual ~Buddy() = 0;
    };

}


#endif //PROJECT_BUDDY_HPP
