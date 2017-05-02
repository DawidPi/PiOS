//
// Created by dapl on 2017-04-27.
//

#include "DynamicAllocator.hpp"
#include "Scheduler.hpp"
#include "PiOS.hpp"
#include "Task.hpp"

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
    setupTaskContext();
}

void PiOSImpl::addTask(RealTimeTask &&task) {
    mScheduler.addRealTimeTask(std::forward<RealTimeTask>(task));
    setupTaskContext();
}

DynamicAllocator &PiOSImpl::allocator() {
    return mAllocator;
}

void PiOSImpl::setupTaskContext() {
    Task &task = mScheduler.fetchNextTask();

    if (&task != mCurrentTask) {
        mCurrentTask = &task;

        if (mCurrentTask != nullptr)
            mCurrentTask->abort();

        task.start();
    }
}
