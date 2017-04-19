//
// Created by dapl on 2017-04-14.
//

#include <gtest/gtest.h>
#include <ListEDF.hpp>

using namespace PiOS;

class ListEDFTest : public ::testing::Test {
public:
    class FlagChangingTask {
    public:
        FlagChangingTask(bool &flag) : mFlag(flag) {}

        void operator()() { mFlag = !mFlag; }

    private:
        bool &mFlag;
    };
};

TEST_F(ListEDFTest, backgroundGetSetTest) {
    ListEDF EDF([]() {});
    bool backgroundFlag = false;
    EDF.setBackgroundTask(Task(FlagChangingTask(backgroundFlag)));
    EDF.fetchNextTask().job()();
    ASSERT_TRUE(backgroundFlag);

    EDF.setBackgroundTask(Task([&backgroundFlag]() { backgroundFlag = true; }));
    EDF.fetchNextTask().job()();
    ASSERT_TRUE(backgroundFlag);
}

TEST_F(ListEDFTest, wrongUsage) {
    ListEDF EDF([]() {});
    bool backgroundFlag = false;
    EDF.setBackgroundTask(Task(FlagChangingTask(backgroundFlag)));

    EDF.addRealTimeTask(RealTimeTask([]() {}, 0_time, 1_time));
    EDF.addRealTimeTask(RealTimeTask([]() {}, 0_time, 1_time));

    // does not remove any task, because no task is pending
    EDF.finishPendingTask();
    EDF.finishPendingTask();
    EDF.finishPendingTask();

    EDF.fetchNextTask().job()();
    ASSERT_FALSE(backgroundFlag);

    EDF.finishPendingTask();
    EDF.fetchNextTask();
    EDF.finishPendingTask();

    bool rtFlag = false;
    EDF.addRealTimeTask(RealTimeTask(FlagChangingTask(rtFlag), 0_time, 1_time));
    EDF.addRealTimeTask(RealTimeTask([]() {}, 0_time, 2_time));

    //keeps fetching same task, because pending one is not finished
    EDF.fetchNextTask().job()();
    ASSERT_TRUE(rtFlag);
    EDF.fetchNextTask().job()();
    ASSERT_FALSE(rtFlag);
    EDF.fetchNextTask().job()();
    ASSERT_TRUE(rtFlag);

    //once finished, another task can be fetched
    EDF.finishPendingTask();
    EDF.fetchNextTask().job()();
    ASSERT_TRUE(rtFlag); //no change, so another task was fetched
}

TEST_F(ListEDFTest, timeout_errorCalled) {
    bool mImmediateErrorCalled = false;
    bool mLateErrorCalled = false;

    ListEDF EDF([&mImmediateErrorCalled]() { mImmediateErrorCalled = true; },
                [&mLateErrorCalled]() { mLateErrorCalled = true; });

    EDF.timeTick(0_time);
    RealTimeTask task([]() {}, 0_time, 3_time);
    EDF.addRealTimeTask(task);

    EDF.timeTick(4_time);
    ASSERT_TRUE(mImmediateErrorCalled);
    ASSERT_TRUE(mLateErrorCalled);
}

TEST_F(ListEDFTest, appendRealTimeTask) {
    ListEDF EDF([]() {});
    EDF.timeTick(0_time);

    //ready tasks appending
    std::array<bool, 5> tasksFlags{false, false, false, false, false};
    EDF.addRealTimeTask(RealTimeTask(FlagChangingTask(tasksFlags[0]), 0_time, 2_time));
    EDF.addRealTimeTask(RealTimeTask(FlagChangingTask(tasksFlags[1]), 0_time, 1_time));
    EDF.addRealTimeTask(RealTimeTask(FlagChangingTask(tasksFlags[2]), 0_time, 7_time));
    EDF.addRealTimeTask(RealTimeTask(FlagChangingTask(tasksFlags[3]), 0_time, 4_time));

    //all tasks should be ready in order (1, 0, 3, 2)
    EDF.fetchNextTask().job()();
    ASSERT_TRUE(tasksFlags[1]);

    //not released, so should return the same task
    tasksFlags[1] = false;
    EDF.fetchNextTask().job()();
    ASSERT_TRUE(tasksFlags[1]);

    for (auto idx : {0, 3, 2}) {
        EDF.finishPendingTask();
        EDF.fetchNextTask().job()();
        ASSERT_TRUE(tasksFlags[idx]);
    }

    EDF.finishPendingTask();
    bool backgroundTaskFlag = false;
    EDF.setBackgroundTask(Task(FlagChangingTask(backgroundTaskFlag)));

    //all real time tasks finished. Time for background task
    EDF.fetchNextTask().job()();
    ASSERT_TRUE(backgroundTaskFlag);
}

TEST_F(ListEDFTest, unreleasedTasks) {
    bool backgroundFlag(false);
    ListEDF EDF([]() {});
    EDF.setBackgroundTask(Task{FlagChangingTask(backgroundFlag)});

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
        EDF.addRealTimeTask(RealTimeTask(FlagChangingTask(currentRelease.second), currentRelease.first, deadlineTime));
    }

    //all tasks are unreleased, background should be returned
    EDF.fetchNextTask().job()();
    ASSERT_TRUE(backgroundFlag);

    std::array<size_t, 7> expectedSequence{4, 0, 6, 1, 3, 2, 5};
    for (auto &expectedIdx : expectedSequence) {
        EDF.finishPendingTask();
        auto newCurrentTime = std::max(currentTime, releasesTimes[expectedIdx].first);
        if (newCurrentTime != currentTime) {
            currentTime = newCurrentTime;
            EDF.timeTick(currentTime);
        }

        EDF.fetchNextTask().job()();
        ASSERT_TRUE(releasesTimes[expectedIdx].second);
    }
    ASSERT_TRUE(
            std::all_of(releasesTimes.begin(), releasesTimes.end(), [](auto &elem) { return elem.second == true; }));
}

TEST_F(ListEDFTest, normalUsageTest) {
    ListEDF EDF([]() {});
    std::vector<char> tasksOutput;

    EDF.addRealTimeTask(RealTimeTask([&tasksOutput]() { tasksOutput.push_back('Z'); }, 0_time, 5_time));
    EDF.addRealTimeTask(RealTimeTask([&tasksOutput]() { tasksOutput.push_back('D'); }, 0_time, 6_time));
    EDF.addRealTimeTask(RealTimeTask([&tasksOutput]() { tasksOutput.push_back('C'); }, 0_time, 7_time));
    EDF.addRealTimeTask(RealTimeTask([&tasksOutput]() { tasksOutput.push_back('B'); }, 1_time, 4_time));
    EDF.addRealTimeTask(RealTimeTask([&tasksOutput]() { tasksOutput.push_back('A'); }, 1_time, 3_time));

    EDF.timeTick(0_time);
    EDF.fetchNextTask().job()();
    EDF.timeTick(0_time);
    EDF.timeTick(1_time);
    EDF.finishPendingTask();
    EDF.fetchNextTask().job()();
    EDF.finishPendingTask();
    EDF.fetchNextTask().job()();
    EDF.finishPendingTask();
    EDF.fetchNextTask().job()();
    EDF.finishPendingTask();
    EDF.timeTick(2_time);
    EDF.fetchNextTask().job()();
    EDF.finishPendingTask();

    decltype(tasksOutput) expectedOutput{'Z', 'A', 'B', 'D', 'C'};
    ASSERT_EQ(tasksOutput, expectedOutput);
}