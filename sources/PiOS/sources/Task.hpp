//
// Created by dapl on 2017-04-14.
//

#ifndef PROJECT_TASK_H
#define PROJECT_TASK_H

#include <functional>
#include <Context.hpp>
#include <type_traits>
#include "CommonTypes.hpp"

namespace PiOS {
    /*!
     * \brief Represents a job, which can be executed by operating system.
     */
    class Task {
    public:

        /*!
         * \brief Typedef for a functional object, that should be executed, when task is started.
         */
        using TaskJob = void (*)();

        /*!
         * \brief Constructor.
         * \param job Task, which should be executed, when the task starts.
         * \param stackSize size of task's stack.
         * \param startUpFunction Startup of the job function.
         */
        explicit Task(TaskJob job, std::size_t stackSize, StartUp startUpFunction = defaultStartUp);

        Task(const Task &rhs) = delete;

        const Task &operator=(const Task &rhs) = delete;

        /*!
         * \brief Move Constructor.
         * \param rhs Right Hand Side of Constructor.
         */
        Task(Task &&rhs);

        /*!
         * \brief Move assignment operator.
         * \param rhs Right Hand Side of comparison.
         * \return Const reference to itself.
         */
        const Task &operator=(Task &&rhs);

        /*!
         * \brief Starts execution of the task.
         */
        void start();

        /*!
         * \brief Fetches functional object, that is executed along with the task.
         * \return Job, which is to be executed.
         */
        TaskJob job() { return mJob; }

        /*!
         * \brief Allows to get startUp function.
         * @return Pointer to the startUp functor.
         */
        StartUp startUpFunction() { return mStartUp; }

        /*!
         * \brief Default startUp Function for the job.
         */
        static void defaultStartUp();

        PiOS::Context& context(){return mContext;}

        /*!
         * \brief Destructor.
         */
        ~Task();

    private:
        TaskJob mJob;
        void *mStackpointer;
        PiOS::Context mContext;
        StartUp mStartUp;

        static_assert(implementsContextInterface<PiOS::Context>::value,
                      "Context interface is not implemented properly. Please see testArch's Context for more info");
        bool mFirstRun = true;
    };
}


#endif //PROJECT_TASK_H
