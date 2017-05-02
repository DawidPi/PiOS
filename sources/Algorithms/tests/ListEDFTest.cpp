//
// Created by dapl on 2017-04-14.
//

#include <gtest/gtest.h>
#include <ListEDF.hpp>

using namespace PiOS;

class ListEDFTest : public ::testing::Test {
};


bool flag = false;

void flagChangingTask() {
    flag = !flag;
}

TEST_F(ListEDFTest, backgroundGetSetTest) {
    ListEDF EDF([]() {});
    EDF.setBackgroundTask(Task(flagChangingTask, 47));
    EDF.fetchNextTask().job()();
    ASSERT_TRUE(flag);

    EDF.setBackgroundTask(Task(flagChangingTask, 47));
    EDF.fetchNextTask().job()();
    ASSERT_FALSE(flag);
}


bool backgroundFlag = false;

void backgroundFlagChanger() {
    backgroundFlag = !backgroundFlag;
}

TEST_F(ListEDFTest, wrongUsage) {
    ListEDF EDF([]() {});
    EDF.setBackgroundTask(Task(backgroundFlagChanger, 47));

    EDF.addRealTimeTask(RealTimeTask([]() {}, 0_time, 1_time, 47));
    EDF.addRealTimeTask(RealTimeTask([]() {}, 0_time, 1_time, 47));

    // does not remove any task, because no task is pending
    EDF.finishPendingTask();
    EDF.finishPendingTask();
    EDF.finishPendingTask();

    EDF.fetchNextTask().job()();
    ASSERT_FALSE(backgroundFlag);

    EDF.finishPendingTask();
    EDF.fetchNextTask();
    EDF.finishPendingTask();

    flag = false;
    EDF.addRealTimeTask(RealTimeTask(flagChangingTask, 0_time, 1_time, 47));
    EDF.addRealTimeTask(RealTimeTask([]() {}, 0_time, 2_time, 47));

    //keeps fetching same task, because pending one is not finished
    EDF.fetchNextTask().job()();
    ASSERT_TRUE(flag);
    EDF.fetchNextTask().job()();
    ASSERT_FALSE(flag);
    EDF.fetchNextTask().job()();
    ASSERT_TRUE(flag);

    //once finished, another task can be fetched
    EDF.finishPendingTask();
    EDF.fetchNextTask().job()();
    ASSERT_TRUE(flag); //no change, so another task was fetched
}

TEST_F(ListEDFTest, timeout_errorCalled) {
    bool mImmediateErrorCalled = false;
    bool mLateErrorCalled = false;

    ListEDF EDF([&mImmediateErrorCalled]() { mImmediateErrorCalled = true; },
                [&mLateErrorCalled]() { mLateErrorCalled = true; });

    EDF.timeTick(0_time);
    RealTimeTask task([]() {}, 0_time, 3_time, 47);
    EDF.addRealTimeTask(std::move(task));

    EDF.timeTick(4_time);
    ASSERT_TRUE(mImmediateErrorCalled);
    ASSERT_TRUE(mLateErrorCalled);
}

std::array<bool, 5> tasksFlags{false, false, false, false, false};

template<std::size_t idx>
void changeTaskInTasksFlag() {
    tasksFlags[idx] = !tasksFlags[idx];
}

TEST_F(ListEDFTest, appendRealTimeTask) {
    ListEDF EDF([]() {});
    EDF.timeTick(0_time);

    //ready tasks appending
    EDF.addRealTimeTask(RealTimeTask(changeTaskInTasksFlag<0>, 0_time, 2_time, 47));
    EDF.addRealTimeTask(RealTimeTask(changeTaskInTasksFlag<1>, 0_time, 1_time, 47));
    EDF.addRealTimeTask(RealTimeTask(changeTaskInTasksFlag<2>, 0_time, 7_time, 47));
    EDF.addRealTimeTask(RealTimeTask(changeTaskInTasksFlag<3>, 0_time, 4_time, 47));

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
    backgroundFlag = false;
    EDF.setBackgroundTask(Task(backgroundFlagChanger, 47));

    //all real time tasks finished. Time for background task
    EDF.fetchNextTask().job()();
    ASSERT_TRUE(backgroundFlag);
}

std::array<std::pair<Time, bool>, 7> releasesTimes{std::make_pair(2_time, false), //quite random
                                                   std::make_pair(4_time, false),
                                                   std::make_pair(10_time, false),
                                                   std::make_pair(5_time, false),
                                                   std::make_pair(1_time, false),
                                                   std::make_pair(140_time, false),
                                                   std::make_pair(3_time, false)};

template<std::size_t idx>
void changeFlagInReleasesTimes() {
    releasesTimes[idx].second = !releasesTimes[idx].second;
}

TEST_F(ListEDFTest, unreleasedTasks) {
    ListEDF EDF([]() {});
    EDF.setBackgroundTask(Task{backgroundFlagChanger, 47});

    Time currentTime(0_time);
    const Time deadlineTime(500_time); //just big enough

    assert(std::all_of(releasesTimes.begin(), releasesTimes.end(),
                       [deadlineTime](auto &elem) { return elem.first < deadlineTime; }));

    EDF.addRealTimeTask(RealTimeTask(changeFlagInReleasesTimes<0>, releasesTimes[0].first, deadlineTime, 47));
    EDF.addRealTimeTask(RealTimeTask(changeFlagInReleasesTimes<1>, releasesTimes[1].first, deadlineTime, 47));
    EDF.addRealTimeTask(RealTimeTask(changeFlagInReleasesTimes<2>, releasesTimes[2].first, deadlineTime, 47));
    EDF.addRealTimeTask(RealTimeTask(changeFlagInReleasesTimes<3>, releasesTimes[3].first, deadlineTime, 47));
    EDF.addRealTimeTask(RealTimeTask(changeFlagInReleasesTimes<4>, releasesTimes[4].first, deadlineTime, 47));
    EDF.addRealTimeTask(RealTimeTask(changeFlagInReleasesTimes<5>, releasesTimes[5].first, deadlineTime, 47));
    EDF.addRealTimeTask(RealTimeTask(changeFlagInReleasesTimes<6>, releasesTimes[6].first, deadlineTime, 47));

    //all tasks are unreleased, background should be returned
    backgroundFlag = false;
    EDF.fetchNextTask().job()();
    ASSERT_TRUE(backgroundFlag);

    std::array<std::size_t, 7> expectedSequence{4, 0, 6, 1, 3, 2, 5};
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

std::vector<char> tasksOutput;

template<char character>
void characterAdder() {
    tasksOutput.push_back(character);
}

TEST_F(ListEDFTest, normalUsageTest) {
    ListEDF EDF([]() {});

    EDF.addRealTimeTask(RealTimeTask(characterAdder<'Z'>, 0_time, 5_time, 47));
    EDF.addRealTimeTask(RealTimeTask(characterAdder<'D'>, 0_time, 6_time, 47));
    EDF.addRealTimeTask(RealTimeTask(characterAdder<'C'>, 0_time, 7_time, 47));
    EDF.addRealTimeTask(RealTimeTask(characterAdder<'B'>, 1_time, 4_time, 47));
    EDF.addRealTimeTask(RealTimeTask(characterAdder<'A'>, 1_time, 3_time, 47));

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