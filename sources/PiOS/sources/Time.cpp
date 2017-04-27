//
// Created by dapl on 2017-04-14.
//

#include "Time.hpp"

using namespace PiOS;

Time::Time(SignedTimeType timeValue) :
        mTime(static_cast<TimeType>(timeValue)) {}

bool Time::operator<(const Time &rhs) const {
    return static_cast<SignedTimeType>(mTime - rhs.mTime) < 0;
}

bool Time::operator>(const Time &rhs) const {
    return static_cast<SignedTimeType>(mTime - rhs.mTime) > 0;
}

bool Time::operator==(const Time &rhs) const {
    return mTime == rhs.mTime;
}

bool Time::operator<=(const Time &rhs) const {
    return !(*this > rhs);
}

bool Time::operator>=(const Time &rhs) const {
    return !(*this < rhs);
}

Time Time::operator+(const Time &rhs) const {
    Time time;
    time.mTime = mTime + rhs.mTime;
    return time;
}

Time Time::operator-(const Time &rhs) const {
    Time time;
    time.mTime = mTime - rhs.mTime;
    return time;
}

Time &Time::operator+=(const Time &rhs) {
    mTime += rhs.mTime;
    return *this;
}

Time &Time::operator-=(const Time &rhs) {
    mTime += rhs.mTime;
    return *this;
}

Time &Time::operator-() {
    mTime = static_cast<TimeType>(-mTime);
    return *this;
}

Time &Time::operator+() {
    return *this;
}
