//
// Created by dawid on 8/11/17.
//

#ifndef PROJECT_STATUSREGISTERMANAGEMENT_HPP
#define PROJECT_STATUSREGISTERMANAGEMENT_HPP


#include <cstdint>

namespace StatusRegister {

    enum class ProcessorMode : uint32_t{
        ABORT       = 0b10111,
        FIQ         = 0b10001,
        IRQ         = 0b10010,
        SVC         = 0b10011,
        SYSTEM      = 0b11111,
        UNDEFINED   = 0b11011,
        USER        = 0b10000
    };

    const char* toString(ProcessorMode processorMode);

    namespace CPSR{ // do not implement changing processor mode. This cannot be implemented in C/C++ (possible only with pure assembly)
        void enableInterrupts();
        ProcessorMode processorMode();
        uint32_t rawValue();
    }

    namespace SPSR{
        void changeProcessorMode(ProcessorMode newMode);
        ProcessorMode processorMode();
        uint32_t rawValue();

        void setRawValue(uint32_t newSpsr);
    }
}


#endif //PROJECT_STATUSREGISTERMANAGEMENT_HPP
