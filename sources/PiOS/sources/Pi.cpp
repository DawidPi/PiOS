//
// Created by dapl on 2017-04-27.
//

#include "DynamicAllocator.hpp"
#include "Scheduler.hpp"
#include "Pi.hpp"

using namespace PiOS;


void PiOSHolder::choosePiOSImplementation(Pi *piObject) {
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
    setupTaskContext();
}

void Pi::addTask(RealTimeTask &&task) {
    mScheduler.addRealTimeTask(std::forward<RealTimeTask>(task));
    setupTaskContext();
}

DynamicAllocator &Pi::allocator() {
    return mAllocator;
}

void Pi::setupTaskContext() {
    Task &task = mScheduler.fetchNextTask();

    if (&task != mCurrentTask) {
        mCurrentTask = &task;
        scheduler().setCurrentJob(mCurrentTask->job());

        if (mCurrentTask != nullptr)
            mCurrentTask->abort();

        task.start();
    }
}