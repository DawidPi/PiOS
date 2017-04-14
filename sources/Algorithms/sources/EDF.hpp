//
// Created by dapl on 2017-04-14.
//

#ifndef PROJECT_EDF_H
#define PROJECT_EDF_H

#include <array>
#include <functional>
#include "RealTimeTask.hpp"

namespace PiOS {

    class EDF {
    public:
        using DeadlineCallback = std::function<void()>;

        EDF(DeadlineCallback deadlineCallback) : mDeadlineFailFallback(deadlineCallback) {};

        virtual void addRealTimeTask(RealTimeTask &task) = 0;

        virtual void setBackGroundTask(Task &task) = 0;

        virtual Task fetchNextTask() =0;

        virtual ~EDF() {};
    protected:
        DeadlineCallback mDeadlineFailFallback;
    };
}


#endif //PROJECT_EDF_H
