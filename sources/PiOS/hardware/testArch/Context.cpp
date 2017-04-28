//
// Created by dapl on 2017-04-28.
//

#include "Context.hpp"


bool PiOS_hardware::Context::mContextStarted = false;
bool PiOS_hardware::Context::mContextSaved = false;

void PiOS_hardware::Context::startContext() {
    mContextStarted = true;
}

void PiOS_hardware::Context::saveContext() {
    mContextSaved = true;
}
