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

        explicit EDF(DeadlineCallback deadlineCallback) : mDeadlineFailFallback(deadlineCallback) {};

        virtual void timeTick(const Time &newTime) =0;

        virtual void addRealTimeTask(const RealTimeTask &task) = 0;

        virtual void setBackgroundTask(const Task &task) = 0;

        virtual Task &fetchNextTask() =0;

        virtual void finishPendingTask() =0;

        virtual ~EDF() {};
    protected:
        DeadlineCallback mDeadlineFailFallback;
    };
}


#endif //PROJECT_EDF_H
