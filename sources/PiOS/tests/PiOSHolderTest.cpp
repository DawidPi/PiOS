//
// Created by dapl on 2017-05-04.
//


#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Mocks.hpp"
#include <PiOS.hpp>
#include <RealTimeTask.hpp>

using namespace PiOS;

class PiOSHolderTest : public ::testing::Test {

};

TEST_F(PiOSHolderTest, basicTests) {
    SchedulerMock scheduler;
    AllocatorMock allocator;

    PiOS::PiOS piOS(allocator, scheduler);

    PiOSHolder::choosePiOSImplementation(&piOS);
    ASSERT_EQ(PiOSHolder::getInstance(), &piOS);
}
