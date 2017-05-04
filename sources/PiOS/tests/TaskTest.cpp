//
// Created by dapl on 2017-05-04.
//

#include <gtest/gtest.h>
#include "../sources/Task.hpp"

using namespace PiOS;

class TaskTest : public ::testing::Test {
};


TEST_F(TaskTest, basicTests) {
    void (*job)() = []() {};
    Task taskTest(job, 47);

    ASSERT_EQ(taskTest.job(), job);
    ASSERT_EQ(taskTest.startUpFunction(), Task::defaultStartUp);
}