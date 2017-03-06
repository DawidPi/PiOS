//
// Created by dawid on 02.03.17.
//

#ifndef PROJECT_SIMPLEBUDDYFACTORY_HPP
#define PROJECT_SIMPLEBUDDYFACTORY_HPP

#include "SimpleBuddy.hpp"

namespace PiOS {

    /*!
     * \brief SimpleBuddyFactory is a class to make creation of SimpleBuddy memory manager easier
     */
    class SimpleBuddyFactory {
    public:
        /*!
         * \brief creates SimpleBuddy algorithm object.
         *
         * Factory creates SimpleBuddy algorithm object with parameters, that can be
         * deduced with given arguments. Factory automatically calculates smallest exponent of
         * expression 2^exp, which states for smallest page of memory managed.
         *
         * @param binaryTreeMemory pointer to the memory, where Binary tree will store it's information.
         * @param binaryTreeMemoryElements size of memory (given in sizeof(size_t)) binaryTreeMemory.
         * @param memory pointer to the beginning of the managed memory by SimpleBuddy algorithm.
         * @param memorySize size of memor Buddy Algorithm is to manage
         * @return SimpleBuddy algorithm object
         */
        PiOS::SimpleBuddy create(size_t *binaryTreeMemory, size_t binaryTreeMemoryElements, void *memory,
                                 size_t memorySize) const;

    private:
        PiOS::NodeId::RankType
        calculateBlockSizeExponent(PiOS::FixedSizeBinaryTree<size_t>::Depth depth, size_t size) const;
    };

}


#endif //PROJECT_SIMPLEBUDDYFACTORY_HPP
