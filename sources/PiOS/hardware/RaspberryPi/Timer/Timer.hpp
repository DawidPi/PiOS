//
// Created by dawid on 8/1/17.
//

#ifndef PROJECT_SYSTEMTIMER_H
#define PROJECT_SYSTEMTIMER_H

#include <cstdint>

namespace Time {

    class TimePeriod{
    public:
        uint32_t getRawValue(){ return mRawValue; }
        static TimePeriod micros(uint32_t micros) {return {micros};}
        static TimePeriod millis(uint32_t millis) {return micros(millis * 1000);}
        static TimePeriod seconds(uint32_t secs) {return millis(secs * 1000);}
        static TimePeriod minutes(uint32_t mins) {return seconds(mins * 60);}
    private:
        TimePeriod(uint32_t rawTime) : mRawValue(rawTime){}
        uint32_t mRawValue;
    };

    class Timer {
    public:
        using Callback = void (*)();

        void init(TimePeriod timePeriod);

        void process();

        void registerCallback(Callback newCallback);

        void delay(TimePeriod period);

    private:
        static Callback mCallback;
        void enableInterrupt();
    };
}


#endif //PROJECT_SYSTEMTIMER_H
