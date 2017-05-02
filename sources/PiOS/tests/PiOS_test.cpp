//
// Created by dapl on 2017-04-27.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <Time.hpp>
#include <RealTimeTask.hpp>
#include <PiOS.hpp>

using namespace PiOS;
using ::testing::_;
using ::testing::ReturnRef;

class PiOSTest : public ::testing::Test {
public:
    class SchedulerMock : public Scheduler {
    public:
        MOCK_METHOD1(timeTick, void(
                const Time&));

        void addRealTimeTask(RealTimeTask &&rval) {
            addRealTimeTaskProxy(rval);
        }

        MOCK_METHOD1(addRealTimeTaskProxy, void(
                RealTimeTask &));

        MOCK_METHOD0(fetchNextTask, Task & ());

        MOCK_METHOD0(finishPendingTask, void());
    };

    class Allocator : public DynamicAllocator {
    public:
        MOCK_METHOD1(allocate, MemorySpace(std::size_t));

        MOCK_METHOD1(deallocate, void(void * ));
    };
};

TEST_F(PiOSTest, basicTests) {
    SchedulerMock mockedScheduler;
    Allocator mockedAllocator;

    PiOSImpl piOS(mockedAllocator, mockedScheduler);

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
    Allocator mockedAllocator;
    PiOSImpl piOS(mockedAllocator, mockedScheduler);

    PiOS::Context::mContextStarted = false;
    PiOS::Context::mContextSaved = false;

    ASSERT_FALSE(PiOS::Context::mContextStarted);
    ASSERT_FALSE(PiOS::Context::mContextSaved);
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

    ASSERT_TRUE(PiOS::Context::mContextStarted);
    ASSERT_TRUE(PiOS::Context::mContextSaved);
#endif

}

