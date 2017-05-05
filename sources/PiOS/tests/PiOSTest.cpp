//
// Created by dapl on 2017-04-27.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <Time.hpp>
#include <RealTimeTask.hpp>
#include <Pi.hpp>
#include <SystemCall.hpp>
#include "Mocks.hpp"

using namespace PiOS;
using ::testing::_;
using ::testing::ReturnRef;

class PiOSTest : public ::testing::Test {
public:

    static bool taskExecuted;
};

bool PiOSTest::taskExecuted = false;

TEST_F(PiOSTest, basicTests) {
    SchedulerMock mockedScheduler;
    AllocatorMock mockedAllocator;

    Pi piOS(mockedAllocator, mockedScheduler);

    Task backgroundTask([]() {}, 0);
    RealTimeTask rtTask([]() {}, 2_time, 3_time, 47);
    EXPECT_CALL(mockedScheduler, timeTick(1_time)).Times(1);
    EXPECT_CALL(mockedScheduler, fetchNextTask())
            .Times(2)
            .WillOnce(ReturnRef(backgroundTask))
            .WillOnce(ReturnRef(rtTask));

    piOS.timeTick();
    EXPECT_CALL(mockedScheduler, addRealTimeTaskProxy(_)).Times(1);
    piOS.addTask(std::move(rtTask));

    ASSERT_EQ(&piOS.allocator(), &mockedAllocator);
}

TEST_F(PiOSTest, contextCallTest) {

#ifndef TEST_CONTEXT
    ASSERT_TRUE(true);
    return;
#else
    SchedulerMock mockedScheduler;
    AllocatorMock mockedAllocator;
    Pi piOS(mockedAllocator, mockedScheduler);

    Context::mContextStarted = false;
    Context::mContextSaved = false;

    ASSERT_FALSE(Context::mContextStarted);
    ASSERT_FALSE(Context::mContextSaved);
    EXPECT_CALL(mockedScheduler, timeTick(_)).Times(2);
    EXPECT_CALL(mockedScheduler, addRealTimeTaskProxy(_)).Times(2);
    RealTimeTask task([]() {}, 2_time, 3_time, 47);
    RealTimeTask task2([]() {}, 3_time, 4_time, 47);
    Task backgroundTask([]() {}, 0);

    EXPECT_CALL(mockedScheduler, fetchNextTask())
            .Times(4)
            .WillOnce(ReturnRef(backgroundTask))
            .WillOnce(ReturnRef(backgroundTask))
            .WillOnce(ReturnRef(task))
            .WillOnce(ReturnRef(task2));

    piOS.addTask(std::move(task));
    piOS.addTask(std::move(task2));

    piOS.timeTick();
    piOS.timeTick();

    ASSERT_TRUE(Context::mContextStarted);
    ASSERT_TRUE(Context::mContextSaved);
#endif

}

TEST_F(PiOSTest, exitAndStartUPCalled) {
#ifndef TEST_CONTEXT
    ASSERT_TRUE(true);
    return;
#else
    Context::mStartUpFunction = nullptr;

    SchedulerMock mockedScheduler;
    AllocatorMock mockedAllocator;

    Pi piOS(mockedAllocator, mockedScheduler);
    PiOSHolder::choosePiImplementation(&piOS);

    ASSERT_EQ(Context::mStartUpFunction, nullptr);

    RealTimeTask task0([]() { PiOSTest::taskExecuted = true; }, 0_time, 3_time, 47);
    ASSERT_EQ(task0.startUpFunction(), Task::defaultStartUp);

    void (*fakeStartUp)()  = []() {};
    RealTimeTask task1([]() {}, 0_time, 4_time, 47, fakeStartUp);
    ASSERT_EQ(task1.startUpFunction(), fakeStartUp);

    // normally one would add the task first, but in tests with mocks it's not needed
    //piOS.addTask(std::move(task0));

    EXPECT_CALL(mockedScheduler, timeTick(1_time))
            .Times(1);
    EXPECT_CALL(mockedScheduler, fetchNextTask())
            .Times(1)
            .WillOnce(ReturnRef(task0));

    piOS.timeTick();

    ASSERT_TRUE(Context::mContextStarted);
    ASSERT_EQ(Context::mStartUpFunction, Task::defaultStartUp);

    SystemCall::exitCalled = false;
    ASSERT_FALSE(SystemCall::exitCalled);
    ASSERT_FALSE(PiOSTest::taskExecuted);

    Task::defaultStartUp();
    ASSERT_TRUE(PiOSTest::taskExecuted);
    ASSERT_TRUE(SystemCall::exitCalled);
#endif
}