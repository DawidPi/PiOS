//
// Created by dapl on 2017-04-27.
//

#include "DynamicAllocator.hpp"
#include "Scheduler.hpp"
#include "Pi.hpp"

using namespace PiOS;

void PiOSHolder::choosePiImplementation(Pi *piObject) {
    mInstance = piObject;
}

Pi *PiOSHolder::getInstance() {
    return mInstance;
}

Pi *PiOS::PiOSHolder::mInstance = nullptr;

Pi::Pi(DynamicAllocator &allocator, Scheduler &scheduler) :
        mAllocator(allocator),
        mScheduler(scheduler),
        mTime(0_time) {
}

void Pi::timeTick() {
    mScheduler.timeTick(++mTime);
}

void Pi::addTask(RealTimeTask *task) {
    mScheduler.addRealTimeTask(task);
}

DynamicAllocator &Pi::allocator() {
    return mAllocator;
}

void Pi::setBackgroundTask(Task *task) {
    mScheduler.setBackgroundTask(task);
}
