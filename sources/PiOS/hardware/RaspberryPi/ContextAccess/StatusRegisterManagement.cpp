//
// Created by dawid on 8/11/17.
//

#include "StatusRegisterManagement.hpp"

extern "C" std::uint32_t _cpsrValue();
extern "C" void _enable_irq();
extern "C" void _cpsrStore(std::uint32_t newcpsr);
extern "C" std::uint32_t _spsrValue();
extern "C" void _spsrStore(std::uint32_t newcpsr);

constexpr uint32_t PROCESSOR_MODE_MASK = 0b11111;

StatusRegister::ProcessorMode StatusRegister::CPSR::processorMode() {
    return static_cast<ProcessorMode>(rawValue() & PROCESSOR_MODE_MASK);
}

uint32_t StatusRegister::CPSR::rawValue() {
    return _cpsrValue();
}

void StatusRegister::CPSR::enableInterrupts() {
    _enable_irq();
}

void StatusRegister::SPSR::changeProcessorMode(StatusRegister::ProcessorMode newMode) {
    auto spsr = rawValue();
    spsr = (spsr & (~PROCESSOR_MODE_MASK)) | static_cast<uint32_t>(newMode);
    _spsrStore(spsr);
}

StatusRegister::ProcessorMode StatusRegister::SPSR::processorMode() {
    return static_cast<ProcessorMode>(rawValue() & PROCESSOR_MODE_MASK);
}

uint32_t StatusRegister::SPSR::rawValue() {
    return _spsrValue();
}

void StatusRegister::SPSR::setRawValue(uint32_t newSpsr) {
    _spsrStore(newSpsr);
}

const char * StatusRegister::toString(StatusRegister::ProcessorMode processorMode) {
    switch (processorMode){
        case ProcessorMode::ABORT:
            return "ABORT MODE";
        case ProcessorMode::FIQ:
            return "FIQ MODE";
        case ProcessorMode::IRQ:
            return "IRQ MODE";
        case ProcessorMode::SVC:
            return "SVC MODE";
        case ProcessorMode::SYSTEM:
            return "SYSTEM MODE";
        case ProcessorMode::UNDEFINED:
            return "UNDEFINED MODE";
        case ProcessorMode::USER:
            return "USER MODE";
    }

    return "";
}
