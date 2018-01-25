//
// Created by dawid on 1/25/18.
//

#include <gtest/gtest.h>
#include <power.hpp>

class PowerTest : public ::testing::Test {

};

TEST_F(PowerTest, testOutput){

    // in mathematics pow(0,0) is undefined and
    // noone should rely on the output of pow(0,0),
    // but it's worth to know if implementation changes
    ASSERT_EQ(PiOS::pow(0,0),   1);

    ASSERT_EQ(PiOS::pow(1,0),   1);
    ASSERT_EQ(PiOS::pow(10,0),  1);
    ASSERT_EQ(PiOS::pow(50,0),  1);
    ASSERT_EQ(PiOS::pow(100,0), 1);

    ASSERT_EQ(PiOS::pow(0,1),   0);
    ASSERT_EQ(PiOS::pow(0,10),  0);
    ASSERT_EQ(PiOS::pow(0,50),  0);
    ASSERT_EQ(PiOS::pow(0,100), 0);

    ASSERT_EQ(PiOS::pow(1,1),   1);
    ASSERT_EQ(PiOS::pow(2,2),   4);
    ASSERT_EQ(PiOS::pow(2,10),  1024);
    ASSERT_EQ(PiOS::pow(3,5),   243);
}
