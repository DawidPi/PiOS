//
// Created by dawid on 31.01.17.
//

#ifndef PROJECT_SCHEDULER_HPP
#define PROJECT_SCHEDULER_HPP

namespace PiOS{
    class Scheduler{
    public:
        virtual void chooseNextThread(ThreadPool& threadPool)=0;
        virtual ~Scheduler() = default;
    };
}

#endif //PROJECT_SCHEDULER_HPP
