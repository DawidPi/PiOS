//
// Created by dapl on 2017-04-14.
//

#include "RealTimeTask.hpp"

using namespace PiOS;

RealTimeTask::RealTimeTask(RealTimeTask &&rhs) :
        Task(std::forward<Task>(rhs)),
        mDeadline(rhs.deadline()),
        mReleaseTime(rhs.releaseTime()) {}