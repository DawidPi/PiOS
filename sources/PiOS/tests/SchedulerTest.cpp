//
// Created by dapl on 2017-05-04.
//

#include <gtest/gtest.h>
#include "../sources/Task.hpp"
#include "Mocks.hpp"

using namespace PiOS;

class SchedulerTest : public ::testing::Test {

};


TEST_F(SchedulerTest, basicTests) {
    SchedulerMock scheduler;

    Task::TaskJob job = []() {};
    scheduler.setCurrentJob(job);

    ASSERT_EQ(scheduler.currentJob(), job);
}