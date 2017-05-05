//
// Created by dapl on 2017-05-04.
//


#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Mocks.hpp"
#include <Pi.hpp>

using namespace PiOS;

class PiOSHolderTest : public ::testing::Test {

};

TEST_F(PiOSHolderTest, basicTests) {
    SchedulerMock scheduler;
    AllocatorMock allocator;

    Pi piOS(allocator, scheduler);

    PiOSHolder::choosePiImplementation(&piOS);
    ASSERT_EQ(PiOSHolder::getInstance(), &piOS);
}
