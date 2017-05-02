//
// Created by dapl on 2017-04-14.
//

#ifndef PROJECT_TASK_H
#define PROJECT_TASK_H

#include <functional>
#include <Context.hpp>
#include <type_traits>

namespace PiOS {
    /*!
     * \brief Represents a job, which can be executed by operating system.
     */
    class Task {
    public:
        /*!
         * \brief Typedef for a functional object, that should be executed, when task is started
         */
        using TaskJob = void (*)();

        /*!
         * \brief Constructor.
         * @param task Task, which should be executed, when the task starts.
         */
        explicit Task(TaskJob task, std::size_t stackSize);

        Task(const Task &rhs) = delete;

        const Task &operator=(const Task &rhs) = delete;

        Task(Task &&rhs);

        const Task &operator=(Task &&rhs);

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

        ~Task();

    private:
        TaskJob mJob;
        void *mStackpointer;
        PiOS::Context mContext;

        static_assert(has_saveContext_method<PiOS::Context>::value,
                      "Context is missing method void saveContext()");
        static_assert(has_startContext_method<PiOS::Context>::value,
                      "Context is missing method void startContext()");
    };
}


#endif //PROJECT_TASK_H
