//
// Created by dapl on 2017-04-14.
//

#include <gtest/gtest.h>
#include <ListEDF.hpp>
#include <vector>

using namespace PiOS;

class ListEDFTest : public ::testing::Test {
};


bool flag = false;

void flagChangingTask() {
    flag = !flag;
}

TEST_F(ListEDFTest, backgroundGetSetTest) {
    ListEDF EDF([]() {});
    EDF.setBackgroundTask(new Task(flagChangingTask, 47));
    EDF.fetchNextTask()->job()();
    ASSERT_TRUE(flag);

    EDF.setBackgroundTask(new Task(flagChangingTask, 47));
    EDF.fetchNextTask()->job()();
    ASSERT_FALSE(flag);
}


bool backgroundFlag = false;

void backgroundFlagChanger() {
    backgroundFlag = !backgroundFlag;
}

TEST_F(ListEDFTest, wrongUsage) {
    ListEDF EDF([]() {});
    EDF.setBackgroundTask(new Task(backgroundFlagChanger, 47));

    EDF.addRealTimeTask(new RealTimeTask([]() {}, 0_time, 1_time, 47));
    EDF.addRealTimeTask(new RealTimeTask([]() {}, 0_time, 1_time, 47));

    // does not remove any task, because no task is pending
    EDF.finishPendingTask();
    EDF.finishPendingTask();
    EDF.finishPendingTask();

    EDF.fetchNextTask()->job()();
    ASSERT_FALSE(backgroundFlag);

    EDF.finishPendingTask();
    EDF.fetchNextTask();
    EDF.finishPendingTask();

    flag = false;
    EDF.addRealTimeTask(new RealTimeTask(flagChangingTask, 0_time, 1_time, 47));
    EDF.addRealTimeTask(new RealTimeTask([]() {}, 0_time, 2_time, 47));

    //keeps fetching same task, because pending one is not finished
    EDF.fetchNextTask()->job()();
    ASSERT_TRUE(flag);
    EDF.fetchNextTask()->job()();
    ASSERT_FALSE(flag);
    EDF.fetchNextTask()->job()();
    ASSERT_TRUE(flag);

    //once finished, another task can be fetched
    EDF.finishPendingTask();
    EDF.fetchNextTask()->job()();
    ASSERT_TRUE(flag); //no change, so another task was fetched
}

TEST_F(ListEDFTest, timeout_errorCalled) {
    bool mImmediateErrorCalled = false;
    bool mLateErrorCalled = false;

    ListEDF EDF([&mImmediateErrorCalled]() { mImmediateErrorCalled = true; },
                [&mLateErrorCalled]() { mLateErrorCalled = true; });

    EDF.timeTick(0_time);
    RealTimeTask task([]() {}, 0_time, 3_time, 47);
    EDF.addRealTimeTask(&task);

    //EDF always keeps track of currently pending closest deadline task, so
    // task must be pending (must be fetched), so that it recognizes proper deadlines

    EDF.fetchNextTask();
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
    EDF.addRealTimeTask(new RealTimeTask(changeTaskInTasksFlag<0>, 0_time, 2_time, 47));
    EDF.addRealTimeTask(new RealTimeTask(changeTaskInTasksFlag<1>, 0_time, 1_time, 47));
    EDF.addRealTimeTask(new RealTimeTask(changeTaskInTasksFlag<2>, 0_time, 7_time, 47));
    EDF.addRealTimeTask(new RealTimeTask(changeTaskInTasksFlag<3>, 0_time, 4_time, 47));

    //all tasks should be ready in order (1, 0, 3, 2)
    EDF.fetchNextTask()->job()();
    ASSERT_TRUE(tasksFlags[1]);

    //not released, so should return the same task
    tasksFlags[1] = false;
    EDF.fetchNextTask()->job()();
    ASSERT_TRUE(tasksFlags[1]);

    for (auto idx : {0, 3, 2}) {
        EDF.finishPendingTask();
        EDF.fetchNextTask()->job()();
        ASSERT_TRUE(tasksFlags[idx]);
    }

    EDF.finishPendingTask();
    backgroundFlag = false;
    EDF.setBackgroundTask(new Task(backgroundFlagChanger, 47));

    //all real time tasks finished. Time for background task
    EDF.fetchNextTask()->job()();
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

std::vector<char> tasksOutput;

template<char character>
void characterAdder() {
    tasksOutput.push_back(character);
}

TEST_F(ListEDFTest, normalUsageTest) {
    ListEDF EDF([]() {});

    EDF.addRealTimeTask(new RealTimeTask(characterAdder<'Z'>, 0_time, 5_time, 47));
    EDF.addRealTimeTask(new RealTimeTask(characterAdder<'D'>, 0_time, 6_time, 47));
    EDF.addRealTimeTask(new RealTimeTask(characterAdder<'C'>, 0_time, 7_time, 47));
    EDF.addRealTimeTask(new RealTimeTask(characterAdder<'B'>, 1_time, 4_time, 47));
    EDF.addRealTimeTask(new RealTimeTask(characterAdder<'A'>, 1_time, 3_time, 47));

    EDF.timeTick(0_time);
    EDF.fetchNextTask()->job()();
    EDF.timeTick(0_time);
    EDF.timeTick(1_time);
    EDF.finishPendingTask();
    EDF.fetchNextTask()->job()();
    EDF.finishPendingTask();
    EDF.fetchNextTask()->job()();
    EDF.finishPendingTask();
    EDF.fetchNextTask()->job()();
    EDF.finishPendingTask();
    EDF.timeTick(2_time);
    EDF.fetchNextTask()->job()();
    EDF.finishPendingTask();

    decltype(tasksOutput) expectedOutput{'Z', 'A', 'B', 'D', 'C'};
    ASSERT_EQ(tasksOutput, expectedOutput);
}

void backgroundJob(){

}

TEST_F(ListEDFTest, chooseBackgroundWhenRTAvailable){
    ListEDF EDF([](){});
    tasksOutput.clear();

    Task backgroundTask(backgroundJob, 10);
    EDF.setBackgroundTask(&backgroundTask);

    EDF.addRealTimeTask(new RealTimeTask(characterAdder<'A'>, 3_time, 5_time, 47));

    auto* fetchedTask = EDF.fetchNextTask();
    ASSERT_EQ(fetchedTask->job(), backgroundJob);

    EDF.timeTick(2_time);
    auto* fetchedTask2 = EDF.fetchNextTask();
    ASSERT_EQ(fetchedTask2->job(), backgroundJob);

    EDF.timeTick(3_time);
    auto* fetchedTask3 = EDF.fetchNextTask();
    ASSERT_EQ(fetchedTask3->job(), characterAdder<'A'>);

}