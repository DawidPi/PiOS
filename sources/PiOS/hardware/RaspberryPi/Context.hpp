//
// Created by dapl on 2017-04-28.
//

#ifndef PROJECT_CONTEXT_HPP
#define PROJECT_CONTEXT_HPP

#include <cstddef>
#include <cstdint>
#include <cstring>
#include "ContextAccess/StatusRegisterManagement.hpp"
#include "../../sources/ContextInterface.hpp"
#include "../../sources/CommonTypes.hpp"

namespace PiOS {
    class Context {
    public:
        using PCPointer = void (*)();
        Context(void *stackPointer, PCPointer function) :
                mSp(reinterpret_cast<uint32_t>(stackPointer)),
                mPC(reinterpret_cast<uint32_t>(function))
        {}

        static constexpr std::size_t minStackSize = 1024ull;

        void startContext(StartUp function);

        void saveContext();

        void setRegisters(std::uint32_t registers[13]){ memcpy(mRegisters, registers, sizeof(mRegisters));}
        void setStackPointer(std::uint32_t stackPointer){mSp = stackPointer; }
        void setLinkRegister(std::uint32_t linkRegister){mLr = linkRegister; }
        void setProgramCounter(std::uint32_t programCounter){mPC = programCounter;}
        void setProcessorStatus(std::uint32_t processorStatus) {mCpsr = processorStatus;}

        std::uint32_t programCounter(){return mPC;}
        std::uint32_t processorStatus();

        const uint32_t *getRegisters() const;

        uint32_t getLr() const;

        uint32_t getSp() const;

        uint32_t getPC() const;

        uint32_t getCpsr() const;

        static size_t getMinStackSize();

    private:
        std::uint32_t mRegisters[13] = {0};
        std::uint32_t mLr = 0;
        std::uint32_t mSp = 0;
        std::uint32_t mPC = 0;
        std::uint32_t mCpsr = 0x600001D0;
    };
}

#endif //PROJECT_CONTEXT_HPP
