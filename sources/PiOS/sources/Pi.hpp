//
// Created by dapl on 2017-04-27.
//

#ifndef PROJECT_PIOS_HPP
#define PROJECT_PIOS_HPP

#include "Time.hpp"
#include "DynamicAllocator.hpp"
#include "Scheduler.hpp"

namespace PiOS {

    class Pi {
    public:
        Pi(DynamicAllocator &allocator, Scheduler &scheduler);

        Pi(const Pi &rhs) = delete;

        Pi &operator=(const Pi &rhs)= delete;

        void timeTick();

        void addTask(RealTimeTask *task);

        void setBackgroundTask(Task *task);

        DynamicAllocator &allocator();

        Scheduler &scheduler() { return mScheduler; };

        Time currentTime(){return mTime;}

    private:
        DynamicAllocator &mAllocator;
        Scheduler &mScheduler;
        Time mTime;

    };

    class PiOSHolder {
    public:
        static void choosePiImplementation(Pi *piObject);

        static Pi *getInstance();

    private:
        static Pi *mInstance;
    };
}


#endif //PROJECT_PIOS_HPP
