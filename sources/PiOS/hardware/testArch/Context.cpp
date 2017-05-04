//
// Created by dapl on 2017-04-28.
//

#include "Context.hpp"

using namespace PiOS;

bool Context::mContextStarted = false;
bool Context::mContextSaved = false;
StartUp Context::mStartUpFunction = nullptr;

void Context::startContext(StartUp startUp) {
    mStartUpFunction = startUp;
    mContextStarted = true;
}

void Context::saveContext() {
    mContextSaved = true;
}
