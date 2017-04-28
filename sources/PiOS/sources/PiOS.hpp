//
// Created by dapl on 2017-04-27.
//

#ifndef PROJECT_PIOS_HPP
#define PROJECT_PIOS_HPP

#include "Time.hpp"
#include "DynamicAllocator.hpp"
#include "Scheduler.hpp"

namespace PiOS {

    class PiOSImpl;

    class PiOSHolder {
    public:
        static void choosePiOSImplementation(PiOSImpl *piOSImplementation);

        static PiOSImpl *getInstance();

    private:
        static PiOSImpl *mInstance;
    };


    class PiOSImpl {
    public:
        PiOSImpl(DynamicAllocator &allocator, Scheduler &scheduler);

        PiOSImpl(const PiOSImpl &rhs) = delete;

        PiOSImpl &operator=(const PiOSImpl &rhs)= delete;

        void timeTick();
        void addTask(const RealTimeTask &task);
        DynamicAllocator &allocator();

    private:
        Task *mCurrentTask = nullptr;
        DynamicAllocator &mAllocator;
        Scheduler &mScheduler;
        Time mTime;

        void setupTaskContext();
    };
}


#endif //PROJECT_PIOS_HPP
