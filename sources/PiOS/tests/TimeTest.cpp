//
// Created by dapl on 2017-04-14.
//

#include <gtest/gtest.h>
#include "../sources/Time.hpp"

using namespace PiOS;

class TimeTest : public ::testing::Test {
public:

};

TEST_F(TimeTest, timeAddition) {
    Time time(1);
    ASSERT_EQ(time.getRawValue(), 1);

    Time time2(2);
    ASSERT_EQ(time2, 2_time);
    ASSERT_EQ(3, (time + time2).getRawValue());

    ASSERT_EQ(-time2, Time(-2));
}

TEST_F(TimeTest, timeComparison) {
    ASSERT_EQ(1_time, 1_time);
    ASSERT_LE(1_time, 2_time);
    ASSERT_GE(2_time, 1_time);

    auto max = std::numeric_limits<Time::TimeType>::max();
    Time maxTime(max);
    Time minTime(static_cast<Time::TimeType>(max + 1));
    Time minTimePlus(static_cast<Time::TimeType>(max + 2));

    ASSERT_GE(minTime, maxTime);
    ASSERT_GE(minTimePlus, minTime);
    ASSERT_LE(maxTime, minTime);
    ASSERT_LE(minTime, minTimePlus);

    Time zeroMinus(-1);
    Time zero(0);
    Time zeroPlus(1);

    ASSERT_LE(zeroMinus, zero);
    ASSERT_GE(zeroPlus, zero);
    ASSERT_GE(zeroPlus, zeroMinus);
    ASSERT_LE(zeroMinus, zeroPlus);

    ASSERT_FALSE(0_time < 0_time);
    ASSERT_FALSE(0_time > 0_time);
    ASSERT_TRUE(0_time >= 0_time);
    ASSERT_TRUE(0_time <= 0_time);
    ASSERT_TRUE(0_time == 0_time);
}

TEST_F(TimeTest, randomComparisonTest) {
    ASSERT_EQ(0_time, 0_time);
    ASSERT_EQ(3000_time, 3000_time);
    ASSERT_EQ(2456_time, 2456_time);
    ASSERT_EQ(-3000_time, -3000_time);
    ASSERT_EQ(-2456_time, -2456_time);

    // this is the maximum difference, that allows proper distinction between tasks. ie.
    // if they are too late or yet fine.
    // there is assumption, that timing of future tasks in no longer than
    // current_time + Time(std::numeric_limits<Time::SignedTimeType>::max()
    // this is caused by integer type overflow

    ASSERT_LE(-1_time, Time(std::numeric_limits<Time::SignedTimeType>::max()));
    ASSERT_LE(Time(std::numeric_limits<Time::SignedTimeType>::max()), -1_time);

    ASSERT_LE(-3000_time, 3000_time);
    ASSERT_LE(-2456_time, 2456_time);


    ASSERT_GE(-2_time, Time(std::numeric_limits<Time::SignedTimeType>::max()));
    ASSERT_GE(3000_time, -3000_time);
    ASSERT_GE(2456_time, -2456_time);
}

TEST_F(TimeTest, incrementTest) {
    auto time = 1_time;
    ASSERT_EQ(time++, 1_time);
    ASSERT_EQ(time, 2_time);
    ASSERT_EQ(++time, 3_time);

    auto timeRandomPlus = 534_time; //fairly random
    ASSERT_EQ(timeRandomPlus++, 534_time);
    ASSERT_EQ(timeRandomPlus, 535_time);
    ASSERT_EQ(++timeRandomPlus, 536_time);

    auto timeRandomMinus = -534_time; //fairly random
    ASSERT_EQ(timeRandomMinus++, -534_time);
    ASSERT_EQ(timeRandomMinus, -533_time);
    ASSERT_EQ(++timeRandomMinus, -532_time);
}