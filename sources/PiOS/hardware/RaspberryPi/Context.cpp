//
// Created by dapl on 2017-04-28.
//

#include "Context.hpp"

using namespace PiOS;

extern Context lastContext;

void Context::startContext(StartUp startUp) {
    mPC = reinterpret_cast<uint32_t>(startUp);
    //lastContext = *this;
}

void Context::saveContext() {
    mPC = lastContext.mPC;
    mLr = lastContext.mLr;
    mSp = lastContext.mSp;
    mCpsr = lastContext.mCpsr;
    memcpy(mRegisters, lastContext.mRegisters, sizeof(mRegisters));

}

uint32_t Context::processorStatus() {
    return mCpsr;
}

size_t Context::getMinStackSize() {
    return minStackSize;
}

const uint32_t *Context::getRegisters() const {
    return mRegisters;
}

uint32_t Context::getLr() const {
    return mLr;
}

uint32_t Context::getSp() const {
    return mSp;
}

uint32_t Context::getPC() const {
    return mPC;
}

uint32_t Context::getCpsr() const {
    return mCpsr;
}