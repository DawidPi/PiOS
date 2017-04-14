//
// Created by dapl on 2017-04-14.
//

#ifndef PROJECT_TASK_H
#define PROJECT_TASK_H

#include <functional>
#include "Context.hpp"

namespace PiOS {
    //todo implement Task
    class Task {
    public:
        using TaskJob = std::function<void()>;

        Task(TaskJob task);

        void start();

        void abort();

        TaskJob job() { return mJob; }

    private:
        TaskJob mJob;
        Context mContext;
    };
}


#endif //PROJECT_TASK_H
