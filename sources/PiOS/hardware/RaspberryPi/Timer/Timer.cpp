//
// Created by dawid on 8/1/17.
//

#include <cassert>
#include <ContextAccess/StatusRegisterManagement.hpp>
#include <Uart/Uart.hpp>
#include "Timer.hpp"
#include "Register.hpp"
#include "TimerRegisters.hpp"

using namespace Time;

void Timer::init(TimePeriod timePeriod) {
    TimerPeriph<Timer0>::LOAD::value(timePeriod.getRawValue());

    using Bits = TimerBits<Timer0>;
    TimerPeriph<Timer0>::CONTROL::value(Bits::ENABLE | Bits::PERIODIC
                                | Bits::BITS_32_COUNTERS | Bits::INTERRUPT_ENABLE);

    enableInterrupt();
}

void Timer::process() {
    //uart::cout << "processing timer" << uart::endl;
    if(TimerPeriph<Timer0>::IRQ_MASKED::value()) {
        TimerPeriph<Timer0>::IRQ_CLR::value(1);
        PICIntEnableClear::value(PICIntEnableBits::Timer0);
        mCallback();
        enableInterrupt();
    }
}

void Timer::registerCallback(Timer::Callback newCallback) {
    assert(newCallback != nullptr);
    mCallback = newCallback;
}

void Timer::delay(TimePeriod period) {
    TimerPeriph<Timer1>::LOAD::value(period.getRawValue());
    using Bits = TimerBits<Timer1>;
    TimerPeriph<Timer1>::CONTROL::value(Bits::ENABLE | Bits::ONESHOT | Bits::BITS_32_COUNTERS);
    while(TimerPeriph<Timer1>::VALUE::value());
}

Timer::Callback Timer::mCallback = [](){};

void Timer::enableInterrupt() {
    PICIntEnable::value(PICIntEnableBits::Timer0);
}
