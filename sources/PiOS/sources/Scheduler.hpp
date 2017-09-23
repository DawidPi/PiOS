//
// Created by dapl on 2017-04-27.
//

#ifndef PROJECT_SCHEDULER_HPP
#define PROJECT_SCHEDULER_HPP

#include "Task.hpp"

namespace PiOS {

    class Task;

    class Time;

    class RealTimeTask;

    class Scheduler {
    public:
        /*!
         * \brief Informs Scheduler object, that some time quantum elapsed.
         * @param newTime Current system time.
         */
        virtual void timeTick(const Time &newTime)=0;

        /*!
         * \brief Adds realTimeTask to the queue.
         * @param task Task to be added.
         */
        virtual void addRealTimeTask(RealTimeTask *task)=0;

        /*!
         * \brief Fetches next task from the Scheduler. If RealTime queue
         * is empty, then background task is executed.
         * @return Task, that should be executed
         */
        virtual Task *fetchNextTask()=0;

        /*!
         * \brief Informs Scheduler, that last task is finished. Should delete
         * last task from the queue. In case, when background task
         * is executed, executing this method will have no side effects.
         */
        virtual void finishPendingTask()=0;

        /*!
         * \brief Informs about current Job, that is currently chosen, by scheduler
         * \return TaskJob, which is being processed
         */
        Task::TaskJob currentJob() { return mCurrentJob; }

        /*!
         * \brief Sets job, to be executed, when context swiches. Should be called only
         * internally by the kernel, not user
         * \param newJob new job, that is to be executed by startUp function.
         */
        void setCurrentJob(Task::TaskJob newJob) { mCurrentJob = newJob; }

        virtual void setBackgroundTask(Task* task)=0;

        /*!
         * \brief Destructor
         */
        virtual ~Scheduler() {};

    private:
        static Task::TaskJob mCurrentJob;
    };
}


#endif //PROJECT_SCHEDULER_HPP
