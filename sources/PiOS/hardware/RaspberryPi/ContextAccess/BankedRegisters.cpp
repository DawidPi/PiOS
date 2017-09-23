//
// Created by dawid on 8/24/17.
//

#include "BankedRegisters.hpp"

extern "C" std::uint32_t _systemStackPointerValue();
extern "C" std::uint32_t _systemLinkRegisterValue();
extern "C" void _systemStackPointerSet(std::uint32_t);
extern "C" void _systemLinkRegisterSet(std::uint32_t);
extern "C" void* _stackPointerValue();

std::uint32_t BankedRegisters::systemStackPointer() {
    return _systemStackPointerValue();
}

std::uint32_t BankedRegisters::systemLinkRegister() {
    return _systemLinkRegisterValue();
}

void BankedRegisters::systemLinkRegister(std::uint32_t newLr) {
    _systemLinkRegisterSet(newLr);
}

void BankedRegisters::systemStackPointer(std::uint32_t newSP) {
    _systemStackPointerSet(newSP);
}

void* BankedRegisters::currentStackPointer() {
    return _stackPointerValue();
}


