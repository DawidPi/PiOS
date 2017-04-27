//
// Created by dapl on 2017-04-27.
//

#ifndef PROJECT_PIOS_HPP
#define PROJECT_PIOS_HPP

#include "Time.hpp"
#include "DynamicAllocator.hpp"
#include "Scheduler.hpp"

namespace PiOS {
    /*!
     * Facade class, that allows to use Operating System.
     */
    class PiOS {
    public:
        PiOS(DynamicAllocator &allocator, Scheduler &scheduler);

        PiOS(const PiOS &rhs) = delete;

        PiOS &operator=(const PiOS &rhs)= delete;

        void timeTick();

        void addTask(const RealTimeTask &task);

        DynamicAllocator &allocator();

    private:
        DynamicAllocator &mAllocator;
        Scheduler &mScheduler;
        Time mTime;
    };
}


#endif //PROJECT_PIOS_HPP
