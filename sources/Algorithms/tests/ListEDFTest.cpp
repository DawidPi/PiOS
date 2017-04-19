//
// Created by dapl on 2017-04-14.
//

#include <gtest/gtest.h>
#include <ListEDF.hpp>

using namespace PiOS;

class ListEDFTest : public ::testing::Test {
public:
    ListEDFTest() : mEDF([]() {}, PiOS::EDF::DeadlineCallback()) {

    }

    class FlagChangingTask {
    public:
        FlagChangingTask(bool &flag) : mFlag(flag) {}

        void operator()() { mFlag = !mFlag; }

    private:
        bool &mFlag;
    };

    ListEDF mEDF;
};

TEST_F(ListEDFTest, backgroundGetSetTest) {
    mEDF = ListEDF([]() {}, PiOS::EDF::DeadlineCallback());
    bool backgroundFlag = false;
    mEDF.setBackgroundTask(Task(FlagChangingTask(backgroundFlag)));
    mEDF.fetchNextTask().job()();
    ASSERT_TRUE(backgroundFlag);

    mEDF.setBackgroundTask(Task([&backgroundFlag]() { backgroundFlag = true; }));
    mEDF.fetchNextTask().job()();
    ASSERT_TRUE(backgroundFlag);
}

TEST_F(ListEDFTest, wrongUsage) {
    mEDF = ListEDF([]() {}, PiOS::EDF::DeadlineCallback());
    bool backgroundFlag = false;
    mEDF.setBackgroundTask(Task(FlagChangingTask(backgroundFlag)));

    mEDF.addRealTimeTask(RealTimeTask([]() {}, 0_time, 1_time));
    mEDF.addRealTimeTask(RealTimeTask([]() {}, 0_time, 1_time));

    mEDF.finishPendingTask();
    mEDF.finishPendingTask();
    mEDF.finishPendingTask();
    // at this point tasks are removed, even though none is pending

    mEDF.fetchNextTask().job()();
    ASSERT_TRUE(backgroundFlag);

    bool rtFlag = false;
    mEDF.addRealTimeTask(RealTimeTask(FlagChangingTask(rtFlag), 0_time, 1_time));
    mEDF.addRealTimeTask(RealTimeTask([]() {}, 0_time, 2_time));

    //keeps fetching same task, because pending one is not finished
    mEDF.fetchNextTask().job()();
    ASSERT_TRUE(rtFlag);
    mEDF.fetchNextTask().job()();
    ASSERT_FALSE(rtFlag);
    mEDF.fetchNextTask().job()();
    ASSERT_TRUE(rtFlag);

    //once finished, another task can be fetched
    mEDF.finishPendingTask();
    mEDF.fetchNextTask().job()();
    ASSERT_TRUE(rtFlag); //no change, so another task was fetched
}

TEST_F(ListEDFTest, timeout_errorCalled) {
    bool mImmediateErrorCalled = false;
    bool mLateErrorCalled = false;

    mEDF = ListEDF([&mImmediateErrorCalled]() { mImmediateErrorCalled = true; },
                   [&mLateErrorCalled]() { mLateErrorCalled = true; });

    mEDF.timeTick(0_time);
    RealTimeTask task([]() {}, 0_time, 3_time);
    mEDF.addRealTimeTask(task);

    mEDF.timeTick(4_time);
    ASSERT_TRUE(mImmediateErrorCalled);
    ASSERT_TRUE(mLateErrorCalled);

    mEDF = ListEDF([]() {}, []() {});
}

TEST_F(ListEDFTest, appendRealTimeTask) {
    mEDF.timeTick(0_time);

    //ready tasks appending
    std::array<bool, 5> tasksFlags{false, false, false, false, false};
    mEDF.addRealTimeTask(RealTimeTask(FlagChangingTask(tasksFlags[0]), 0_time, 2_time));
    mEDF.addRealTimeTask(RealTimeTask(FlagChangingTask(tasksFlags[1]), 0_time, 1_time));
    mEDF.addRealTimeTask(RealTimeTask(FlagChangingTask(tasksFlags[2]), 0_time, 7_time));
    mEDF.addRealTimeTask(RealTimeTask(FlagChangingTask(tasksFlags[3]), 0_time, 4_time));

    //all tasks should be ready in order (1, 0, 3, 2)
    mEDF.fetchNextTask().job()();
    ASSERT_TRUE(tasksFlags[1]);

    //not released, so should return the same task
    tasksFlags[1] = false;
    mEDF.fetchNextTask().job()();
    ASSERT_TRUE(tasksFlags[1]);

    for (auto idx : {0, 3, 2}) {
        mEDF.finishPendingTask();
        mEDF.fetchNextTask().job()();
        ASSERT_TRUE(tasksFlags[idx]);
    }

    mEDF.finishPendingTask();
    bool backgroundTaskFlag = false;
    mEDF.setBackgroundTask(Task(FlagChangingTask(backgroundTaskFlag)));

    //all real time tasks finished. Time for background task
    mEDF.fetchNextTask().job()();
    ASSERT_TRUE(backgroundTaskFlag);
    mEDF = ListEDF([]() {}, PiOS::EDF::DeadlineCallback());
}

TEST_F(ListEDFTest, unreleasedTasks) {
    bool backgroundFlag(false);
    mEDF.setBackgroundTask(Task{FlagChangingTask(backgroundFlag)});

    Time currentTime(0_time);
    std::array<std::pair<Time, bool>, 7> releasesTimes{std::make_pair(2_time, false), //quite random
                                                       std::make_pair(4_time, false),
                                                       std::make_pair(10_time, false),
                                                       std::make_pair(5_time, false),
                                                       std::make_pair(1_time, false),
                                                       std::make_pair(140_time, false),
                                                       std::make_pair(3_time, false)};
    const Time deadlineTime(500_time); //just big enough

    assert(std::all_of(releasesTimes.begin(), releasesTimes.end(),
                       [deadlineTime](auto &elem) { return elem.first < deadlineTime; }));

    for (auto &currentRelease : releasesTimes) {
        mEDF.addRealTimeTask(RealTimeTask(FlagChangingTask(currentRelease.second), currentRelease.first, deadlineTime));
    }

    //all tasks are unreleased, background should be returned
    mEDF.fetchNextTask().job()();
    ASSERT_TRUE(backgroundFlag);

    std::array<size_t, 7> expectedSequence{4, 0, 6, 1, 3, 2, 5};
    for (auto &expectedIdx : expectedSequence) {
        mEDF.finishPendingTask();
        auto newCurrentTime = std::max(currentTime, releasesTimes[expectedIdx].first);
        if (newCurrentTime != currentTime) {
            currentTime = newCurrentTime;
            this->mEDF.timeTick(currentTime);
        }

        mEDF.fetchNextTask().job()();
        ASSERT_TRUE(releasesTimes[expectedIdx].second);
    }
    ASSERT_TRUE(
            std::all_of(releasesTimes.begin(), releasesTimes.end(), [](auto &elem) { return elem.second == true; }));
    mEDF = ListEDF([]() {}, PiOS::EDF::DeadlineCallback());
}