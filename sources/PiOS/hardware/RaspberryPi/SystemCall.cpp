//
// Created by dapl on 2017-05-04.
//

#include "SystemCall.hpp"

using namespace PiOS;

void SystemCall::operator()(SystemCallType sysCall) {
    assert(sysCall == SystemCallType::EXIT);

    auto Pi = PiOS::PiOSHolder::getInstance();
    assert(Pi != nullptr);

    asm("svc 0"); //exit
}
