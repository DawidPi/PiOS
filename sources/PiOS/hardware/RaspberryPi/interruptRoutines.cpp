//
// Created by dawid on 8/4/17.
//

#include <Pi.hpp>
#include <Task.hpp>
#include <ContextAccess/BankedRegisters.hpp>
#include <cassert>
#include "Uart/Uart.hpp"
#include "Timer/Timer.hpp"
#include "StatusRegisterManagement.hpp"

namespace {
    PiOS::Task *currentTask = nullptr;
    PiOS::Context lastContext(nullptr, nullptr);

    void restoreContext(uint32_t *savedRegisters) {
        constexpr auto enableInterrupts = ~0x80;
        StatusRegister::SPSR::setRawValue(lastContext.getCpsr() & enableInterrupts);

        memcpy(savedRegisters, lastContext.getRegisters(), sizeof(uint32_t) * 13);
        BankedRegisters::systemStackPointer(lastContext.getSp());
        BankedRegisters::systemLinkRegister(lastContext.getLr());
    }

    void saveContext(uint32_t returnPC, uint32_t *savedRegisters) {
        lastContext.setRegisters(savedRegisters);
        lastContext.setProgramCounter(returnPC - 4);
        lastContext.setProcessorStatus(StatusRegister::SPSR::rawValue());
        lastContext.setStackPointer(BankedRegisters::systemStackPointer());
        lastContext.setLinkRegister(BankedRegisters::systemLinkRegister());

        if (currentTask != nullptr) {
            currentTask->context() = lastContext;
        }
    }
}

extern "C" {

//return return address
uint32_t c_irq_interrupt(uint32_t returnPC, uint32_t savedRegisters[13]) {
    // irq comes only from timer
    Time::Timer timer;
    timer.process();
    namespace SPSR = StatusRegister::SPSR;
    namespace CPSR = StatusRegister::CPSR;
    if (SPSR::processorMode() == StatusRegister::ProcessorMode::USER) {
        saveContext(returnPC, savedRegisters);

        auto Pi = PiOS::PiOSHolder::getInstance();
        assert(Pi);
        Pi->timeTick();

        auto *task = Pi->scheduler().fetchNextTask();
        if (task != currentTask) {
            currentTask = task;
            currentTask->start();
            lastContext = currentTask->context();
            Pi->scheduler().setCurrentJob(task->job());
        }

        returnPC = lastContext.programCounter();
        restoreContext(savedRegisters);
    }
    return returnPC;
}

std::uint32_t c_swi_interrupt(uint32_t savedRegisters[13]) {
    uart::cout << "c_swi_interrupt()" << uart::endl;

    namespace SPSR = StatusRegister::SPSR;
    namespace CPSR = StatusRegister::CPSR;
    assert(SPSR::processorMode() == StatusRegister::ProcessorMode::USER);
    auto Pi = PiOS::PiOSHolder::getInstance();

    if (currentTask != nullptr)
        Pi->scheduler().finishPendingTask();

    auto *task = Pi->scheduler().fetchNextTask();

    assert(task != currentTask);

    currentTask = task;
    currentTask->start();
    lastContext = currentTask->context();
    Pi->scheduler().setCurrentJob(task->job());

    auto returnPC = lastContext.programCounter();
    restoreContext(savedRegisters);

    return returnPC;
}

void c_reset_interrupt() {
    uart::cout << "c_reset_interrupt()" << uart::endl;
}

void c_undefined_interrupt() {
    uart::cout << "c_undefined_interrupt()" << uart::endl;
}

void c_prefetch_interrupt() {
    uart::cout << "c_prefetch_interrupt()" << uart::endl;
}

void c_data_interrupt() {
    uart::cout << "c_data_interrupt()" << uart::endl;
}

void c_unused_interrupt() {
    uart::cout << "c_unused_interrupt()" << uart::endl;
}

void c_fiq_interrupt() {
    uart::cout << "c_fiq_interrupt()" << uart::endl;
}


}
