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

    class PiOSHolder {
    public:
        static void choosePiOSImplementation(Pi *piObject);

        static Pi *getInstance();

    private:
        static Pi *mInstance;
    };
}


#endif //PROJECT_PIOS_HPP
