//
// Created by dawid on 31.01.17.
//

#ifndef PROJECT_THREADPOOL_HPP
#define PROJECT_THREADPOOL_HPP

namespace PiOS {
    class ThreadPool {
    public:
        void addThread(Thread&& thread);
        virtual ~ThreadPool()=0;
    };
}

#endif //PROJECT_THREADPOOL_HPP
