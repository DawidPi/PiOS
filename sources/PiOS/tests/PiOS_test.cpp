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

class PiOSTest : public ::testing::Test {
public:
    class SchedulerMock : public Scheduler {
    public:
        MOCK_METHOD1(timeTick, void(
                const Time&));

        MOCK_METHOD1(addRealTimeTask, void(
                const RealTimeTask&));

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

    PiOS::PiOS piOs(mockedAllocator, mockedScheduler);

    EXPECT_CALL(mockedScheduler, timeTick(1_time)).Times(1);
    piOs.timeTick();

    RealTimeTask task([]() {}, 2_time, 3_time);
    EXPECT_CALL(mockedScheduler, addRealTimeTask(_)).Times(1);
    piOs.addTask(task);

    ASSERT_EQ(&piOs.allocator(), &mockedAllocator);
}