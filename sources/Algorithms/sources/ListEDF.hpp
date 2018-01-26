//
// Created by dapl on 2017-04-14.
//

#ifndef PROJECT_HEAPEDF_HPP
#define PROJECT_HEAPEDF_HPP

#include "EDF.hpp"
#include "Task.hpp"
#include "List.hpp"
#include <list>

namespace PiOS {
    /*!
     * \brief Implementation of \ref EDF interface using Lists for managing queues.
     */
    class ListEDF : public EDF {
        template<typename T>
        using ListType = std::list<T>;
        //todo check if can be replaced with PiOS::RealTimeTask
        using TaskList = ListType<PiOS::RealTimeTask*>;
    public:
        /*!
         * \brief Constructor. Computational complexity O(1).
         * @param deadlineCallback, function, which is to be executed immediatelly, when any deadline is missing.
         * @param postFailCallback, function, which is to be executed, when deadlineCallback finishes work, and
         *         queues are cleaned
         */
        explicit ListEDF(DeadlineCallback deadlineCallback, DeadlineCallback postFailCallback = []() {});

        ListEDF(const ListEDF &rhs) = delete;

        ListEDF &operator=(const ListEDF &rhs) = delete;


        /*!
         * \brief Informs EDF, about time slice elapsed.
         *
         * Method, when executed, will update EDF's local time.
         * Method also checks whether tasks should be released,
         * and if so, removes from waiting queue and appends tasks
         * to ready queue. This operation is O(m) complexity, where
         * m is umber of tasks in waiting state and n is number
         * of tasks in ready state.
         *
         * Method checks, whether deadlines are not missed.
         * If deadline is missed, it executes deadline callback,
         * later clears the queues and finally executes postFailCallback,
         * to be able to start work again.
         *
         * @param newTime New, current system time. Cannot be smaller than
         * previosuly given system time.
         */
        void timeTick(const Time &newTime) override;

        /*!
         * \brief Adds RealTimeTask to queue.
         *
         * If release time is greater, than current time. Task will be added
         * to the waiting queue, otherwise to the ready queue. Complexity
         * of the operation is O(1)(in case of ready tasks) or O(n)(in case of waiting tasks),
         *  where n is number of tasks in the queue
         * (queue of ready or waiting tasks, depending on mentioned condition above.)
         *
         * @param task RealTimeTask, which is to be added. It's release time, cannot
         * be smaller, than deadline time and deadline cannot be smaller, than current
         * system time.
         */
        void addRealTimeTask(RealTimeTask *task) override;

        /*!
         * \brief Sets task, that is to be executed, when no other task is left in the
         * queue. By default this task is empty, so by default, task, which does nothing,
         * will be executed. In case of
         *
         * @param task Background task, that is to be executed, when there is no ready
         * RealTimeTask set.
         */
        void setBackgroundTask(Task *task) override { mBackgroundTask = task; }

        /*!
         * \brief Indicates, that last fetched Task is finished, and therefore should be removed
         * from the queue.
         */
        void finishPendingTask() override;

        /*!
         * \brief Fetches next task to be executed. Takes O(n) time complexity, where n is number of tasks
         * @return Task, which is meant to be executed.
         */
        Task* fetchNextTask() override;

    private:
        TaskList mRTUnreleasedTasks;
        TaskList mRTReadyTasksList;
        Task* mBackgroundTask;
        Time mCurrentTime;
        bool mBackgroundIsPending;
        using ListIterator = TaskList::iterator;
        ListIterator mPendingRTTask;

    private:
        bool isPendingTaskValid() const;

        void updateCurrentTime(const PiOS::Time &newTime) { mCurrentTime = newTime; }

        TaskList::iterator findLastInvalidWaitingTask(const PiOS::Time &newTime);
    };
}

#endif //PROJECT_HEAPEDF_HPP
