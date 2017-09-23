//
// Created by dawid on 8/23/17.
//

#ifndef PROJECT_TIMERREGISTERS_HPP
#define PROJECT_TIMERREGISTERS_HPP

namespace {

using Timer0 = RegisterBase<0x101E2000>;
using Timer1 = RegisterBase<0x101E2020>;
using Timer2 = RegisterBase<0x101E2020>;
using Timer3 = RegisterBase<0x101E2020>;

template <typename TimerBase>
struct TimerRegisters{
    using LOAD = Register<TimerBase, 0x00>;
    using VALUE = Register<TimerBase, 0x04>;
    using CONTROL = Register<TimerBase, 0x08>;
    using IRQ_CLR = Register<TimerBase, 0x0C>;
    using IRQ_RAW = Register<TimerBase, 0x10>;
    using IRQ_MASKED = Register<TimerBase, 0x14>;
    using RELOAD = Register<TimerBase, 0x18>;


    struct ControlBits{
        static constexpr uint32_t ONESHOT = 1;
        static constexpr uint32_t BITS_32_COUNTERS = 1 << 1;
        static constexpr uint32_t INTERRUPT_ENABLE = 1 << 5;
        static constexpr uint32_t ENABLE = 1 << 7;
        static constexpr uint32_t PERIODIC = 1 << 6;
    };
};

template <typename Timer>
using TimerPeriph = TimerRegisters<Timer>;
template <typename Timer>
using TimerBits = typename TimerRegisters<Timer>::ControlBits;

using PICBase = RegisterBase<0x10140000>;
using PICIntEnable = Register<PICBase, 0x10>;
using PICIntEnableClear = Register<PICBase, 0x14>;

struct PICIntEnableBits{
    static constexpr uint32_t Timer0 = 0x10;
};

}

#endif //PROJECT_TIMERREGISTERS_HPP
