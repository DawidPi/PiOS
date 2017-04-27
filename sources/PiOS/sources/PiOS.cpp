//
// Created by dapl on 2017-04-27.
//

#include "DynamicAllocator.hpp"
#include "Scheduler.hpp"
#include "PiOS.hpp"

using namespace PiOS;

PiOS::PiOS::PiOS(DynamicAllocator &allocator, Scheduler &scheduler) :
        mAllocator(allocator),
        mScheduler(scheduler),
        mTime(0) {}

void PiOS::PiOS::timeTick() {
    mScheduler.timeTick(++mTime);
}

void PiOS::PiOS::addTask(const RealTimeTask &task) {
    mScheduler.addRealTimeTask(task);
}

DynamicAllocator &PiOS::PiOS::allocator() {
    return mAllocator;
}
