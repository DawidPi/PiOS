//
// Created by dapl on 2017-04-14.
//

#ifndef PROJECT_EDF_H
#define PROJECT_EDF_H

//#include <array>
#include <functional>
#include <Scheduler.hpp>
#include "RealTimeTask.hpp"

namespace PiOS {

    /*!
     * \brief Interface for Earliest Deadline First algorithm.
     *
     * Earliest Deadline First interface represents implementation,
     * where 2 kinds of Tasks are served. Those are \ref RealTimeTask
     * and \ref Task.
     *
     * This interface makes implementation possible to manage two
     * queues of Tasks and one backgroundTask. One queue of tasks
     * would be dedicated for Tasks, which were not released. Queue
     * is ordered ascending with it's start Times. Second Queue is
     * for already released time and is sorted ascending considering
     * deadlines.
     */
    class EDF : public Scheduler {
    public:
        /*!
         * \brief Type of callback, which will be invoked, when deadline is missed
         */
        using DeadlineCallback = std::function<void()>;

        /*!
         * \brief Constructor.
         * @param immediateCallback callback, which will be invoked, when any of the deadline is missed
         */
        explicit EDF(DeadlineCallback immediateCallback, DeadlineCallback postFailCallback = []() {}) :
                mDeadlineFailFallback(immediateCallback),
                mPostFailCallback(postFailCallback) {};

        /*!
         * \brief Sets background task for the EDF. Task will be
         * executed, when real time queue is empty.
         * @param task
         */
        virtual void setBackgroundTask(Task *task) = 0;
        virtual ~EDF() {};

    protected:
        DeadlineCallback mDeadlineFailFallback;
        DeadlineCallback mPostFailCallback;
    };
}


#endif //PROJECT_EDF_H
