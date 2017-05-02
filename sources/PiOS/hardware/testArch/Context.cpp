//
// Created by dapl on 2017-04-28.
//

#include "Context.hpp"


bool PiOS::Context::mContextStarted = false;
bool PiOS::Context::mContextSaved = false;

void PiOS::Context::startContext() {
    mContextStarted = true;
}

void PiOS::Context::saveContext() {
    mContextSaved = true;
}
