//
// Created by dapl on 2017-04-28.
//

#ifndef PROJECT_CONTEXT_HPP
#define PROJECT_CONTEXT_HPP

#include <cstddef>
#include "../../sources/ContextInterface.hpp"
#include "../../sources/CommonTypes.hpp"

#define TEST_CONTEXT

namespace PiOS {
    class Context {
    public:
        static constexpr std::size_t minStackSize = 47ull;

        using PCPointer = void (*)();

        Context(void *stackPointer, PCPointer function) :
                mStackPointer(stackPointer),
                mPC(function) {}

        void startContext(StartUp function);

        void saveContext();

    private:
        void *mStackPointer;
        PCPointer mPC;

    public:
        static StartUp mStartUpFunction;
        static bool mContextStarted;
        static bool mContextSaved;
    };
}

#endif //PROJECT_CONTEXT_HPP
