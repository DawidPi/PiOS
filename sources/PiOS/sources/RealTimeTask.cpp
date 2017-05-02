//
// Created by dapl on 2017-04-14.
//

#include "RealTimeTask.hpp"

using namespace PiOS;

RealTimeTask::RealTimeTask(RealTimeTask &&task) :
        Task(std::forward<Task>(task)),
        mDeadline(task.deadline()),
        mReleaseTime(task.releaseTime()) {}