//
// Created by dawid on 31.01.17.
//

#ifndef PROJECT_PIOS_HPP
#define PROJECT_PIOS_HPP

#include "Scheduler.hpp"
#include "Thread.hpp"

/*!
 * \brief namespace with PiOS API
 */
namespace PiOS {

    /*!
     * \brief Main manager of Operating system
     */
    class PiOS {
    public:
        PiOS() = delete;

        /*!
         * PiOS constructor. For OS to be able to work it must know
         * the scheduling algorithm and threads to launch.
         * @param scheduler Scheduler algorithm to be used.
         * @param threadPool
         */
        PiOS(Scheduler &&scheduler) : mScheduler(scheduler) {};

        /*!
         * Adds new thread to current scheduler
         * @param thread new thread, which is to be appended to scheduler
         */
        void addThread(const Thread &thread);

        /*!
         * Changes currently used scheduler algorithm.
         * @param scheduler new scheduler algorithm
         */
        void changeScheduler(Scheduler &scheduler);

    private:
        Scheduler &mScheduler;
    };
}


#endif //PROJECT_PIOS_HPP
