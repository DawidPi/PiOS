//
// Created by dapl on 2017-04-27.
//

#include "DynamicAllocator.hpp"
#include "Scheduler.hpp"
#include "PiOS.hpp"

using namespace PiOS;


void PiOSHolder::choosePiOSImplementation(PiOSImpl *piOSImplementation) {
    mInstance = piOSImplementation;
}

PiOSImpl *PiOSHolder::getInstance() {
    return mInstance;
}

PiOSImpl *PiOS::PiOSHolder::mInstance = nullptr;

PiOSImpl::PiOSImpl(DynamicAllocator &allocator, Scheduler &scheduler) :
        mAllocator(allocator),
        mScheduler(scheduler),
        mTime(0_time) {

}

void PiOSImpl::timeTick() {
    mScheduler.timeTick(++mTime);
}

void PiOSImpl::addTask(const RealTimeTask &task) {
    mScheduler.addRealTimeTask(task);
}

DynamicAllocator &PiOSImpl::allocator() {
    return mAllocator;
}
