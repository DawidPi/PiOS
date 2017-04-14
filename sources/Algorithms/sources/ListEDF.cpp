//
// Created by dapl on 2017-04-14.
//

#include "ListEDF.hpp"
#include <algorithm>
#include <cassert>

PiOS::ListEDF::ListEDF(PiOS::EDF::DeadlineCallback deadlineCallback) :
        EDF(deadlineCallback),
        mRTUnreleasedTasks(),
        mRTTasksList(),
        mBackgroundTask([]() {}),
        mCurrentTime(0) {}

void PiOS::ListEDF::timeTick(const PiOS::Time &newTime) {
    mCurrentTime = newTime;
    auto emptyJob = std::function<void()>();
    RealTimeTask compareTask(emptyJob, newTime, newTime);
    auto lastItem = std::upper_bound(mRTUnreleasedTasks.begin(), mRTUnreleasedTasks.end(), compareTask,
                                     RealTimeTask::RTTaskComparator(RealTimeTask::RTTaskComparator::RELEASE_TIME));
    std::for_each(mRTUnreleasedTasks.begin(), lastItem, [this](auto &elem) { this->addRealTimeTask(elem); });

    if (newTime > mRTTasksList.begin()->deadline()) {
        mDeadlineFailFallback();
    }
}

void PiOS::ListEDF::addRealTimeTask(RealTimeTask &task) {
    assert(task.releaseTime() < task.deadline());
    assert(task.deadline() > mCurrentTime);

    if (task.releaseTime() > mCurrentTime) {
        auto placeToInsert = std::upper_bound(mRTUnreleasedTasks.begin(), mRTUnreleasedTasks.end(), task,
                                              RealTimeTask::RTTaskComparator(
                                                      RealTimeTask::RTTaskComparator::RELEASE_TIME));
        mRTUnreleasedTasks.insert(placeToInsert, task);
    } else {
        auto placeToInsert = std::upper_bound(mRTTasksList.begin(), mRTTasksList.end(), task,
                                              RealTimeTask::RTTaskComparator(RealTimeTask::RTTaskComparator::DEADLINE));
        mRTTasksList.insert(placeToInsert, task);
    }
}


PiOS::Task PiOS::ListEDF::fetchNextTask() {
    if (mRTTasksList.begin() == mRTTasksList.end())
        return Task(mBackgroundTask.job());

    const Task returnTask = Task(mRTTasksList.begin()->job());
    mRTTasksList.erase(mRTTasksList.begin());
    return returnTask;
}