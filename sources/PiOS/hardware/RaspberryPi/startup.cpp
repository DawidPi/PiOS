#include <Pi.hpp>
#include <ListEDF.hpp>
#include <TLSF.hpp>
#include <Timer/Timer.hpp>
#include <ContextAccess/BankedRegisters.hpp>
#include <TLSFExternal.hpp>
#include "Uart/Uart.hpp"
#include "SystemCall.hpp"

#define NOINIT __attribute__((section(".noinit")))

extern char __heap_start__;
extern char __heap_end__;
extern char __static_heap_start__;
extern char __static_heap_end__;
extern unsigned char __bss_start;
extern unsigned char __bss_end;
using Initializer = void (*)();
extern Initializer __init_array_start;
extern Initializer __init_array_end;

void *__current_static_heap NOINIT;

extern void backgroundTask();

namespace {


    //PiOS::DefaultTLSF TLSFMemoryManager(MemorySpace(&__heap_start__, &__heap_end__));
    TLSFExternal TLSFMemoryManager(&__heap_start__, &__heap_end__);
    PiOS::ListEDF EarliestDeadlineScheduler([]() { uart::cout << "EDF time failed"; },
                                                   []() { uart::cout << "EDF time failed"; });
    PiOS::Pi piObject(TLSFMemoryManager, EarliestDeadlineScheduler);

    inline void static_init() {
        uart::cout << "static_init()" << uart::endl;
        __current_static_heap = &__static_heap_start__;

        //initialize BSS section
        for (unsigned char *currentBSSByte = &__bss_start; currentBSSByte < & __bss_end; currentBSSByte++) {
            *currentBSSByte = 0;
        }

        //call global object's constructors
        for (auto initializer = &__init_array_start; initializer < &__init_array_end; ++initializer){
            (*initializer)();
        }
    }

    void initOS() {
        uart::cout << "initOS()" << uart::endl;
        PiOSHolder::choosePiImplementation(&piObject);
    }
}

extern "C" {

int main();

[[noreturn]] void __startup() {
    static_init();
    initOS();

    uart::cout << "main()" << uart::endl;
    main();

    auto currentCPSR = StatusRegister::CPSR::rawValue();
    currentCPSR = (currentCPSR & ~0b11111) | static_cast<std::uint32_t>(StatusRegister::ProcessorMode::USER);
    constexpr auto enableInterrupts = ~0x80;
    currentCPSR = currentCPSR & enableInterrupts;
    asm("msr cpsr_c, %0;" : : "r" (currentCPSR));
    asm volatile("svc 0");
    while(1);

    while(1);
}

}