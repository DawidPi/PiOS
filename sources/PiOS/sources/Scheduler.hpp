//
// Created by dawid on 31.01.17.
//

#ifndef PROJECT_SCHEDULER_HPP
#define PROJECT_SCHEDULER_HPP

#include "Thread.hpp"

namespace PiOS{
    class Scheduler{
    public:
        using size_t=unsigned long;

        virtual size_t addThread(const Thread &thread)=0;

        virtual bool removeThread(size_t threadId)=0;

        virtual bool changeThread(size_t threadId, Thread &newThread)=0;
        virtual ~Scheduler() = default;
    };
}

#endif //PROJECT_SCHEDULER_HPP
