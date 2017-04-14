//
// Created by dapl on 2017-04-14.
//

#ifndef PROJECT_TIME_HPP
#define PROJECT_TIME_HPP

#include <cstdlib>
#include <cstdint>


namespace PiOS {

    /*
     * Time implemented based on ICTOH circular time from
     * "An Efficient Time Representation for Real-Time Embedded Systems"
     * by Giorgio C. Buttazzo and Alessio Carlini
     */
    class Time {
    public:
        using TimeType = uint16_t;
        using SignedTimeType = int16_t;

        Time(SignedTimeType timeValue = 0);

        TimeType getRawValue() const { return mTime; }

        const Time &operator=(TimeType newValue) {
            mTime = newValue;
            return *this;
        }

        Time &operator++() {
            mTime++;
            return *this;
        };

        Time operator++(int) { return Time(static_cast<TimeType>(mTime + 1)); }

        Time operator+(const Time &rhs) const;

        Time operator-(const Time &rhs) const;

        Time &operator+=(const Time &rhs);

        Time &operator-=(const Time &rhs);

        Time &operator-();

        Time &operator+();


        bool operator<(const Time &rhs) const;

        bool operator>(const Time &rhs) const;

        bool operator==(const Time &rhs) const;

        bool operator<=(const Time &rhs) const;

        bool operator>=(const Time &rhs) const;

    private:
        TimeType mTime;
    };

    inline Time operator "" _time(unsigned long long timeValue) {
        return Time(static_cast<uint16_t>(timeValue));
    }
}


#endif //PROJECT_TIME_HPP
