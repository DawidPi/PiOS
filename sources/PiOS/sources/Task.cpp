//
// Created by dapl on 2017-04-14.
//

#include "Task.hpp"

PiOS::Task::Task(PiOS::Task::TaskJob task) :
        mJob(task),
        mContext() {}
