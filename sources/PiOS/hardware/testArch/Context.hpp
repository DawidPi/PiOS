//
// Created by dapl on 2017-04-28.
//

#ifndef PROJECT_CONTEXT_HPP
#define PROJECT_CONTEXT_HPP

#include <cstddef>
#include <cstdint>
#include "../../sources/ContextInterface.hpp"
#include "../../sources/CommonTypes.hpp"

#define TEST_CONTEXT

namespace PiOS {
    class Context {
    public:
        static constexpr std::size_t minStackSize = 47ull;

        using PCPointer = void(*)();

        Context(void *stackPointer, PCPointer function) :
                mStackPointer(stackPointer),
                mPC(reinterpret_cast<uint64_t>(function)) {}

        void startContext(StartUp function);

        void saveContext();

        std::uint32_t programCounter(){return static_cast<std::uint32_t>(mPC);}
        void setProgramCounter(std::uint32_t){}

    private:
        void *mStackPointer;
        std::uint64_t mPC;

    public:
        static StartUp mStartUpFunction;
        static bool mContextStarted;
        static bool mContextSaved;
    };
}

#endif //PROJECT_CONTEXT_HPP
