//
// Created by dapl on 2017-04-14.
//

#include "Task.hpp"

PiOS::Task::Task(TaskJob task, std::size_t stackSize) :
        mJob(task),
        mStackpointer(::operator new(stackSize)),
        mContext(mStackpointer, task) {
}

PiOS::Task::Task(PiOS::Task &&rhs) :
        mJob(rhs.mJob),
        mStackpointer(rhs.mStackpointer),
        mContext(rhs.mContext) {
    rhs.mStackpointer = nullptr;
}

void PiOS::Task::start() {
    mContext.startContext();
}

void PiOS::Task::abort() {
    mContext.saveContext();
}

PiOS::Task::~Task() {
    ::operator delete(mStackpointer);
}

const PiOS::Task &PiOS::Task::operator=(Task &&rhs) {
    mJob = rhs.mJob;
    ::operator delete(mStackpointer);
    mStackpointer = rhs.mStackpointer;
    rhs.mStackpointer = nullptr;

    mContext = rhs.mContext;

    return *this;
}
