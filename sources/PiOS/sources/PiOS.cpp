//
// Created by dapl on 2017-04-27.
//

#include "DynamicAllocator.hpp"
#include "Scheduler.hpp"
#include "PiOS.hpp"

using namespace PiOS;


void PiOSHolder::choosePiOSImplementation(PiOS *piOSImplementation) {
    mInstance = piOSImplementation;
}

::PiOS::PiOS *PiOSHolder::getInstance() {
    return mInstance;
}

::PiOS::PiOS *PiOS::PiOSHolder::mInstance = nullptr;

PiOS::PiOS::PiOS(DynamicAllocator &allocator, Scheduler &scheduler) :
        mAllocator(allocator),
        mScheduler(scheduler),
        mTime(0_time) {
}

void PiOS::PiOS::timeTick() {
    mScheduler.timeTick(++mTime);
    setupTaskContext();
}

void PiOS::PiOS::addTask(RealTimeTask &&task) {
    mScheduler.addRealTimeTask(std::forward<RealTimeTask>(task));
    setupTaskContext();
}

DynamicAllocator &PiOS::PiOS::allocator() {
    return mAllocator;
}

void PiOS::PiOS::setupTaskContext() {
    Task &task = mScheduler.fetchNextTask();

    if (&task != mCurrentTask) {
        mCurrentTask = &task;
        scheduler().setCurrentJob(mCurrentTask->job());

        if (mCurrentTask != nullptr)
            mCurrentTask->abort();

        task.start();
    }
}