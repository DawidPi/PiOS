//
// Created by dapl on 2017-04-14.
//

#include <SystemCall.hpp>
#include "Task.hpp"
#include "Pi.hpp"


PiOS::Task::Task(TaskJob job, std::size_t stackSize, StartUp startUpFunction) :
        mJob(job),
        mStackpointer(::operator new(stackSize)),
        mContext((char*)mStackpointer + stackSize, job),
        mStartUp(startUpFunction) {
}

PiOS::Task::Task(::PiOS::Task &&rhs) :
        mJob(rhs.mJob),
        mStackpointer(rhs.mStackpointer),
        mContext(rhs.mContext),
        mStartUp(rhs.startUpFunction()),
        mFirstRun(rhs.mFirstRun){
    rhs.mStackpointer = nullptr;
}

void PiOS::Task::start() {
    //todo check if task was launched before
    // (everytime started from the beginnign)
    if(mFirstRun){
        mFirstRun = false;
        mContext.setProgramCounter(reinterpret_cast<uint64_t>(startUpFunction()));
    }
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
    mFirstRun = rhs.mFirstRun;

    return *this;
}

void PiOS::Task::defaultStartUp() {
    auto &scheduler = PiOSHolder::getInstance()->scheduler();

    scheduler.currentJob()();

    SystemCall sysCall;
    sysCall(SystemCallType::EXIT);
}
