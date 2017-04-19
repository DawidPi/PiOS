//
// Created by dapl on 2017-04-14.
//

#ifndef PROJECT_TIME_HPP
#define PROJECT_TIME_HPP

#include <cstdlib>
#include <cstdint>


namespace PiOS {

    /*!
     * \brief Time representation for the OS.
     *
     * Time implemented based on ICTOH circular time from
     * "An Efficient Time Representation for Real-Time Embedded Systems"
     * by Giorgio C. Buttazzo and Alessio Carlini
     */
    class Time {
    public:
        /*!
         * \brief Unsigned Time Type for 16bit time representation in the OS.
         */
        using TimeType = uint16_t;
        /*!
         * \brief Signed Time Type for 16bit time representation in the OS.
         */
        using SignedTimeType = int16_t;

        /*!
         * \brief Constructor. takes Signed Time Type as it's initial value.
         * @param timeValue time raw value.
         */
        explicit Time(SignedTimeType timeValue = 0);

        /*!
         * \brief Allows to get raw value of internal time representation.
         * @return Unsigned Raw Value of the internal time.
         */
        TimeType getRawValue() const { return mTime; }

        const Time &operator=(TimeType newValue) {
            mTime = newValue;
            return *this;
        }

        const Time &operator++() {
            mTime++;
            return *this;
        };

        Time operator++(int) { return Time(static_cast<TimeType>(mTime++)); }

        Time operator+(const Time &rhs) const;

        Time operator-(const Time &rhs) const;

        Time &operator+=(const Time &rhs);

        Time &operator-=(const Time &rhs);

        Time &operator-();

        Time &operator+();


        bool operator<(const Time &rhs) const;

        bool operator>(const Time &rhs) const;

        bool operator==(const Time &rhs) const;

        bool operator!=(const Time &rhs) const { return !(*this == rhs); };

        bool operator<=(const Time &rhs) const;

        bool operator>=(const Time &rhs) const;

    private:
        TimeType mTime;
    };

    /*!
     * \brief User Defined Literal for time.
     * @param timeValue Raw value of unsigned Time.
     * @return Time object.
     */
    inline Time operator "" _time(unsigned long long timeValue) {
        return Time(static_cast<uint16_t>(timeValue));
    }
}


#endif //PROJECT_TIME_HPP
