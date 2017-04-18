//
// Created by dapl on 2017-04-14.
//

#ifndef PROJECT_HEAPEDF_HPP
#define PROJECT_HEAPEDF_HPP

#include "EDF.hpp"
#include <list>
#include "Task.hpp"

namespace PiOS {
    class ListEDF : public EDF {
    public:
        explicit ListEDF(DeadlineCallback deadlineCallback);

        void timeTick(const Time &newTime) override;

        void addRealTimeTask(const RealTimeTask &task) override;

        void setBackgroundTask(const Task &task) override { mBackgroundTask = task; }

        void finishPendingTask() override;

        Task &fetchNextTask() override;

    private:
        std::list<RealTimeTask> mRTUnreleasedTasks;
        std::list<RealTimeTask> mRTTasksList;
        Task mBackgroundTask;
        Time mCurrentTime;
        bool mBackgroundIsPending;
    };
}

#endif //PROJECT_HEAPEDF_HPP
