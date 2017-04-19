//
// Created by dapl on 2017-04-14.
//

#ifndef PROJECT_EDF_H
#define PROJECT_EDF_H

#include <array>
#include <functional>
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
    class EDF {
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
         * \brief Informs EDF object, that some time quantum elapsed.
         * @param newTime Current system time.
         */
        virtual void timeTick(const Time &newTime) =0;

        /*!
         * \brief Adds realTimeTask to the queue.
         * @param task Task to be added.
         */
        virtual void addRealTimeTask(const RealTimeTask &task) = 0;

        /*!
         * \brief Sets background task for the EDF. Task will be
         * executed, when real time queue is empty.
         * @param task
         */
        virtual void setBackgroundTask(const Task &task) = 0;

        /*!
         * \brief Fetches next task from the EDF. If RealTime queue
         * is empty, then background task is executed.
         * @return Task, that should be executed
         */
        virtual Task &fetchNextTask() =0;

        /*!
         * \brief Informs EDF, that last task is finished. Should delete
         * last task from the queue. In case, when background task
         * is executed, executing this method will have no side effects.
         */
        virtual void finishPendingTask() =0;

        virtual ~EDF() {};

    protected:
        DeadlineCallback mDeadlineFailFallback;
        DeadlineCallback mPostFailCallback;
    };
}


#endif //PROJECT_EDF_H
