//
// Created by dapl on 2017-04-27.
//

#ifndef PROJECT_PIOS_HPP
#define PROJECT_PIOS_HPP

#include "Time.hpp"
#include "DynamicAllocator.hpp"
#include "Scheduler.hpp"

namespace PiOS {

    class PiOS;

    class PiOSHolder {
    public:
        static void choosePiOSImplementation(PiOS *piOSImplementation);

        static PiOS *getInstance();

    private:
        static PiOS *mInstance;
    };

    class PiOS {
    public:
        PiOS(DynamicAllocator &allocator, Scheduler &scheduler);

        PiOS(const PiOS &rhs) = delete;

        PiOS &operator=(const PiOS &rhs)= delete;

        void timeTick();

        void addTask(RealTimeTask &&task);
        DynamicAllocator &allocator();

        Scheduler &scheduler() { return mScheduler; };

    private:
        Task *mCurrentTask = nullptr;
        DynamicAllocator &mAllocator;
        Scheduler &mScheduler;
        Time mTime;

        void setupTaskContext();
    };
}


#endif //PROJECT_PIOS_HPP
