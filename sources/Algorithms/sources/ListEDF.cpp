//
// Created by dapl on 2017-04-14.
//

#include "ListEDF.hpp"
#include <algorithm>
#include <cassert>

PiOS::ListEDF::ListEDF(DeadlineCallback deadlineCallback, DeadlineCallback postFailCallback) :
        EDF(deadlineCallback, postFailCallback),
        mRTUnreleasedTasks(),
        mRTReadyTasksList(),
        mBackgroundTask(nullptr),
        mCurrentTime(0),
        mBackgroundIsPending(false),
        mPendingRTTask(mRTReadyTasksList.end()) {}


void PiOS::ListEDF::timeTick(const PiOS::Time &newTime) {
    assert(newTime >= mCurrentTime);

    bool pendingTaskIsValid = isPendingTaskValid();
    bool shouldRTTaskBeUpdated = not pendingTaskIsValid;
    updateCurrentTime(newTime);

    auto lastItem = findLastInvalidWaitingTask(newTime);
    std::for_each(mRTUnreleasedTasks.begin(), lastItem, [this](auto &elem) {
        mRTReadyTasksList.push_front(elem);
    });

    mRTUnreleasedTasks.erase(mRTUnreleasedTasks.begin(), lastItem);

    if (shouldRTTaskBeUpdated){
        mPendingRTTask = mRTReadyTasksList.end();
    }

    if (pendingTaskIsValid and newTime > (*mPendingRTTask)->deadline()) {
        mDeadlineFailFallback();
        mRTUnreleasedTasks.clear();
        mRTReadyTasksList.clear();
        mPostFailCallback();
    }
}

void PiOS::ListEDF::addRealTimeTask(RealTimeTask *task) {
    assert(task->releaseTime() < task->deadline());
    assert(task->deadline() > mCurrentTime);

    bool shouldRTTaskBeUpdated = not isPendingTaskValid();

    if (task->releaseTime() > mCurrentTime) {
        auto placeToInsert = std::upper_bound(mRTUnreleasedTasks.begin(), mRTUnreleasedTasks.end(), task->deadline(),
                                              RealTimeTask::RTTaskComparator(
                                                      RealTimeTask::RTTaskComparator::RELEASE_TIME));
        mRTUnreleasedTasks.emplace(placeToInsert, task);
    } else {
        mRTReadyTasksList.push_front(task);
    }

    if (shouldRTTaskBeUpdated){
        mPendingRTTask = mRTReadyTasksList.end();
    }
}


PiOS::Task *PiOS::ListEDF::fetchNextTask() {
    if (mRTReadyTasksList.empty()) {
        mBackgroundIsPending = true;
        return mBackgroundTask;
    }

    mBackgroundIsPending = false;
    if (not isPendingTaskValid()) {
        auto closestDeadlineTask = std::min_element(mRTReadyTasksList.begin(), mRTReadyTasksList.end(),
                                                    [](const RealTimeTask *el1, const RealTimeTask *el2) {
                                                        return el1->deadline() < el2->deadline();
                                                    });
        mPendingRTTask = closestDeadlineTask;
        return *(closestDeadlineTask);
    }

    return *mPendingRTTask;
}

void PiOS::ListEDF::finishPendingTask() {
    if (mBackgroundIsPending)
        return;

    const bool pendingTaskCanBeErased = not mRTReadyTasksList.empty() and mPendingRTTask != mRTReadyTasksList.end();
    if (pendingTaskCanBeErased) {
        mRTReadyTasksList.erase(mPendingRTTask);
        mPendingRTTask = mRTReadyTasksList.end();
    }
}

PiOS::ListEDF::ListType<PiOS::RealTimeTask*>::iterator PiOS::ListEDF::findLastInvalidWaitingTask(const PiOS::Time &newTime) {
    auto lastItem = std::upper_bound(mRTUnreleasedTasks.begin(), mRTUnreleasedTasks.end(), newTime,
                                     PiOS::RealTimeTask::RTTaskComparator(
                                             PiOS::RealTimeTask::RTTaskComparator::RELEASE_TIME));
    return lastItem;
}

bool PiOS::ListEDF::isPendingTaskValid() const { return this->mPendingRTTask != this->mRTReadyTasksList.end(); }
