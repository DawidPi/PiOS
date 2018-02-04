//
// Created by dawid on 8/4/17.
//

#ifndef PROJECT_RASPBERRYPERIPHBASE_HPP
#define PROJECT_RASPBERRYPERIPHBASE_HPP

#include <cstdint>

template <uint32_t base>
struct RegisterBase{
    using Type = RegisterBase<base>;
    static constexpr uint32_t baseValue = base;
    static volatile constexpr auto* const address = reinterpret_cast<uint32_t *>(base);
    static uint32_t value(){return *address;}
    static void value(uint32_t newValue){*address = newValue;}
};

template <typename BaseRegister, unsigned int offset=0>
struct Register{
    using Type = Register<BaseRegister, offset>;
    using Base = RegisterBase<BaseRegister::baseValue + offset>;
    static constexpr uint32_t baseValue = BaseRegister::baseValue + offset;

    static volatile constexpr auto const address = baseValue;
    static uint32_t value(){return *reinterpret_cast<volatile uint32_t *>(address);}
    static void value(uint32_t newValue){*reinterpret_cast<volatile uint32_t *>(address) = newValue;}
};

template<typename REGISTER>
struct RegisterBits {
};


#endif //PROJECT_RASPBERRYPERIPHBASE_HPP
