//
// Created by dapl on 2017-04-14.
//

#ifndef PROJECT_HEAPEDF_HPP
#define PROJECT_HEAPEDF_HPP

#include "EDF.hpp"
#include <list>

namespace PiOS {
    class ListEDF : public EDF {
    public:
        ListEDF(DeadlineCallback deadlineCallback);

        void timeTick(const Time &newTime);

        void addRealTimeTask(RealTimeTask &task) override;

        void setBackGroundTask(Task &task) override { mBackgroundTask = task; }

        Task fetchNextTask() override;

    private:
        std::list<RealTimeTask> mRTUnreleasedTasks;
        std::list<RealTimeTask> mRTTasksList;
        Task mBackgroundTask;
        Time mCurrentTime;
    };
}

#endif //PROJECT_HEAPEDF_HPP
