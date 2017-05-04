//
// Created by dapl on 2017-04-14.
//

#include <SystemCall.hpp>
#include "Task.hpp"
#include "PiOS.hpp"


PiOS::Task::Task(TaskJob job, std::size_t stackSize, StartUp startUpFunction) :
        mJob(job),
        mStackpointer(::operator new(stackSize)),
        mContext(mStackpointer, job),
        mStartUp(startUpFunction) {
}

PiOS::Task::Task(::PiOS::Task &&rhs) :
        mJob(rhs.mJob),
        mStackpointer(rhs.mStackpointer),
        mContext(rhs.mContext),
        mStartUp(rhs.startUpFunction()) {
    rhs.mStackpointer = nullptr;
}

void PiOS::Task::start() {
    mContext.startContext(startUpFunction());
}

void PiOS::Task::abort() {
    mContext.saveContext();
}

PiOS::Task::~Task() {
    ::operator delete(mStackpointer);
}

const ::PiOS::Task &PiOS::Task::operator=(Task &&rhs) {
    mJob = rhs.mJob;

    ::operator delete(mStackpointer);
    mStackpointer = rhs.mStackpointer;
    rhs.mStackpointer = nullptr;

    mContext = rhs.mContext;

    return *this;
}

void PiOS::Task::defaultStartUp() {
    auto &scheduler = PiOSHolder::getInstance()->scheduler();

    scheduler.currentJob()();

    SystemCall sysCall;
    sysCall(SystemCallType::EXIT);
}
