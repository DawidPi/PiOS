//
// Created by dapl on 2017-04-14.
//

#include "ListEDF.hpp"
#include <algorithm>
#include <cassert>
#include <iostream>

PiOS::ListEDF::ListEDF(DeadlineCallback deadlineCallback, DeadlineCallback postFailCallback) :
        EDF(deadlineCallback, postFailCallback),
        mRTUnreleasedTasks(),
        mRTTasksList(),
        mBackgroundTask([]() {}, Context::minStackSize),
        mCurrentTime(0),
        mBackgroundIsPending(false),
        mPendingRTTask(mRTTasksList.end()) {}


void PiOS::ListEDF::timeTick(const PiOS::Time &newTime) {
    assert(newTime >= mCurrentTime);

    bool shouldRTTaskBeUpdated = not isPendingTaskValid();
    updateCurrentTime(newTime);

    auto lastItem = findLastInvalidWaitingTask(newTime);
    std::for_each(mRTUnreleasedTasks.begin(), lastItem, [this](auto &elem) { this->addRealTimeTask(std::move(elem)); });
    mRTUnreleasedTasks.erase(mRTUnreleasedTasks.begin(), lastItem);

    if (shouldRTTaskBeUpdated)
        mPendingRTTask = mRTTasksList.end();

    if (newTime > mRTTasksList.begin()->deadline()) {
        mDeadlineFailFallback();
        mRTUnreleasedTasks.clear();
        mRTTasksList.clear();
        mPostFailCallback();
    }
}

void PiOS::ListEDF::addRealTimeTask(RealTimeTask &&task) {
    assert(task.releaseTime() < task.deadline());
    assert(task.deadline() > mCurrentTime);

    bool shouldRTTaskBeUpdated = not isPendingTaskValid();

    if (task.releaseTime() > mCurrentTime) {
        auto placeToInsert = std::upper_bound(mRTUnreleasedTasks.begin(), mRTUnreleasedTasks.end(), task,
                                              RealTimeTask::RTTaskComparator(
                                                      RealTimeTask::RTTaskComparator::RELEASE_TIME));
        mRTUnreleasedTasks.emplace(placeToInsert, std::forward<RealTimeTask>(task));
    } else {
        auto placeToInsert = std::upper_bound(mRTTasksList.begin(), mRTTasksList.end(), task,
                                              RealTimeTask::RTTaskComparator(RealTimeTask::RTTaskComparator::DEADLINE));
        mRTTasksList.emplace(placeToInsert, std::forward<RealTimeTask>(task));
    }

    if (shouldRTTaskBeUpdated)
        mPendingRTTask = mRTTasksList.end();
}


PiOS::Task &PiOS::ListEDF::fetchNextTask() {
    if (mRTTasksList.empty()) {
        mBackgroundIsPending = true;
        return mBackgroundTask;
    }

    mBackgroundIsPending = false;
    if (not isPendingTaskValid()) {
        mPendingRTTask = mRTTasksList.begin();
        return *mRTTasksList.begin();
    }

    return *mPendingRTTask;
}

void PiOS::ListEDF::finishPendingTask() {
    if (mBackgroundIsPending)
        return;

    const bool pendingTaskCanBeErased = not mRTTasksList.empty() and mPendingRTTask != mRTTasksList.end();
    if (pendingTaskCanBeErased) {
        mRTTasksList.erase(mPendingRTTask);
        mPendingRTTask = mRTTasksList.end();
    }
}

std::list<PiOS::RealTimeTask>::iterator PiOS::ListEDF::findLastInvalidWaitingTask(const PiOS::Time &newTime) {
    auto emptyJob = []() {};
    PiOS::RealTimeTask compareTask(emptyJob, newTime, newTime, 0);
    auto lastItem = std::upper_bound(mRTUnreleasedTasks.begin(), mRTUnreleasedTasks.end(), compareTask,
                                     PiOS::RealTimeTask::RTTaskComparator(
                                             PiOS::RealTimeTask::RTTaskComparator::RELEASE_TIME));
    return lastItem;
}

bool PiOS::ListEDF::isPendingTaskValid() const { return (this->mPendingRTTask != this->mRTTasksList.end()); }
