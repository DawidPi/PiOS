//
// Created by dapl on 2017-04-28.
//

#ifndef PROJECT_CONTEXT_HPP
#define PROJECT_CONTEXT_HPP

#include "../../sources/ContextInterface.hpp"
#include <cstddef>

#define TEST_CONTEXT

namespace PiOS {
    using namespace PiOS;

    class Context {
    public:

        static constexpr std::size_t minStackSize = 47ull;

        using PCPointer = void (*)();

        Context(void *stackPointer, PCPointer function) :
                mStackPointer(stackPointer),
                mPC(function) {}

        void startContext();

        void saveContext();

    private:
        void *mStackPointer;
        PCPointer mPC;

    public:
        static bool mContextStarted;
        static bool mContextSaved;
    };
}


#endif //PROJECT_CONTEXT_HPP
