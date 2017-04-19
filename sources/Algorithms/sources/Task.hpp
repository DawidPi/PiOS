//
// Created by dapl on 2017-04-14.
//

#ifndef PROJECT_TASK_H
#define PROJECT_TASK_H

#include <functional>
#include "Context.hpp"

namespace PiOS {
    /*!
     * \brief Represents a job, which can be executed by operating system.
     */
    class Task {
    public:
        /*!
         * \brief Typedef for a functional object, that should be executed, when task is started
         */
        using TaskJob = std::function<void()>;

        /*!
         * \brief Constructor.
         * @param task Task, which should be executed, when the task starts.
         */
        explicit Task(TaskJob task);

        /*!
         * \brief Starts execution of the task.
         */
        void start();

        /*!
         * \brief Suspends execution of the task.
         */
        void abort();

        /*!
         * \brief Fetches functional object, that is executed along with the task.
         * @return Job, which is to be executed.
         */
        TaskJob job() { return mJob; }

    private:
        TaskJob mJob;
        Context mContext;
    };
}


#endif //PROJECT_TASK_H
