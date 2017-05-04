//
// Created by dapl on 2017-04-14.
//

#ifndef PROJECT_REALTIMETASK_H
#define PROJECT_REALTIMETASK_H

#include "Task.hpp"
#include "Time.hpp"

namespace PiOS {
    /*!
     * \brief Class, which represents RealTimeTask, that can be performed by the OS
     */
    class RealTimeTask : public Task {
    public:
        /*!
         * \brief Functional object, that makes comparison of the tasks possible.
         *
         * Class makes possible to compare Tasks using it's deadline and release times.
         * Useable for sorting with std::sorts
         */
        class RTTaskComparator {
        public:
            enum CompareType {
                RELEASE_TIME, DEADLINE
            };

            /*!
             * \brief Constructor, which decides, how the comparator will be comparing the tasks.
             * \param compareType Type of comparison, that Comparator is to be performed.
             */
            explicit RTTaskComparator(CompareType compareType) : mCompareType(compareType) {}

            /*!
             * \brief Function operator, which performs comparison.
             *
             * Returns true if lhs is less than rhs, false otherwise (acocording to standard
             * comparison operators).
             *
             * @param lhs Left Hand Side of comparison
             * @param rhs Right Hand Side of comparison
             * @return Returns true if lhs is less than rhs, false otherwise.
             */
            bool operator()(const RealTimeTask &lhs, const RealTimeTask &rhs) {
                if (mCompareType == RELEASE_TIME)
                    return lhs.releaseTime() < rhs.releaseTime();
                else
                    return lhs.deadline() < rhs.deadline();
            }

        private:
            CompareType mCompareType;
        };

        /*!
         * \brief Constructor. Creates RealTimeTask with RealTime constants required for proper scheduling.
         *
         * \param job Job to be executed by the Task.
         * \param startTime Release time of the Task.
         * \param deadline Deadline of the Task.
         * \param stackSize Size of the task's stack.
         * \param startUpFunction StartUp of the job. Usually calls current job and exit system call.
         */
        explicit RealTimeTask(const TaskJob &job, const Time &startTime, const Time &deadline, std::size_t stackSize,
                              StartUp startUpFunction = Task::defaultStartUp)
                : Task(job, stackSize, startUpFunction),
                  mDeadline(deadline),
                  mReleaseTime(startTime) {}

        RealTimeTask(const RealTimeTask &rhs) = delete;

        /*!
         * \brief Move constructor
         * @param rhs Right Hand Side of constructor.
         */
        RealTimeTask(RealTimeTask &&rhs);

        /*!
         * \brief Allows to get release time of the Task.
         * @return Release time of the Task.
         */
        const Time &releaseTime() const { return mReleaseTime; }

        /*!
         * \brief Allows to get deadline time of the Task.
         * @return Deadline time of the Task.
         */
        const Time &deadline() const { return mDeadline; }

    private:
        Time mDeadline;
        Time mReleaseTime;
    };
}


#endif //PROJECT_REALTIMETASK_H
