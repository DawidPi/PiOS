//
// Created by dapl on 2017-04-14.
//

#ifndef PROJECT_REALTIMETASK_H
#define PROJECT_REALTIMETASK_H

#include "Task.hpp"
#include "Time.hpp"

namespace PiOS {
    class RealTimeTask : public Task {
    public:
        class RTTaskComparator {
        public:
            enum CompareType {
                RELEASE_TIME, DEADLINE
            };

            RTTaskComparator(CompareType compareType) : mCompareType(compareType) {}

            bool operator()(const RealTimeTask &lhs, const RealTimeTask &rhs) {
                if (mCompareType == RELEASE_TIME)
                    return lhs.releaseTime() < rhs.releaseTime();
                else
                    return lhs.deadline() < rhs.deadline();
            }

        private:
            CompareType mCompareType;
        };

        RealTimeTask(const TaskJob &task, const Time &startTime, const Time &deadline)
                : Task(task),
                  mDeadline(deadline),
                  mReleaseTime(startTime) {}

        const Time &releaseTime() const { return mReleaseTime; }

        const Time &deadline() const { return mDeadline; }

        bool operator<(const RealTimeTask &rhs) const { return deadline() < rhs.deadline(); }

        bool operator>(const RealTimeTask &rhs) const { return deadline() > rhs.deadline(); }

        bool operator==(const RealTimeTask &rhs) const { return deadline() == rhs.deadline(); }

        bool operator<=(const RealTimeTask &rhs) const { return deadline() <= rhs.deadline(); }

        bool operator>=(const RealTimeTask &rhs) const { return deadline() >= rhs.deadline(); }

    private:
        Time mDeadline;
        Time mReleaseTime;
    };
}


#endif //PROJECT_REALTIMETASK_H
