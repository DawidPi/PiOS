//
// Created by dapl on 2017-04-27.
//

#ifndef PROJECT_SCHEDULER_HPP
#define PROJECT_SCHEDULER_HPP

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
        virtual void addRealTimeTask(const RealTimeTask &task)=0;

        /*!
         * \brief Fetches next task from the Scheduler. If RealTime queue
         * is empty, then background task is executed.
         * @return Task, that should be executed
         */
        virtual Task &fetchNextTask()=0;

        /*!
         * \brief Informs Scheduler, that last task is finished. Should delete
         * last task from the queue. In case, when background task
         * is executed, executing this method will have no side effects.
         */
        virtual void finishPendingTask()=0;

        virtual ~Scheduler() {};
    };
}


#endif //PROJECT_SCHEDULER_HPP
