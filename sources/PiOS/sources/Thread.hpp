//
// Created by dawid on 31.01.17.
//

#ifndef PROJECT_THREAD_HPP
#define PROJECT_THREAD_HPP

#include <functional>

namespace PiOS{
    class Thread{
    public:
        using ThreadTask=std::function<void()>;
        using ContextPtr=void*;

        Thread()=delete;
        Thread(const Thread&)=delete;
        Thread(ThreadTask& task) : mTask(task){}
        Thread(ThreadTask&& task) : mTask(task){}

        ContextPtr& context(){return mContext;}
        const ContextPtr& context()const {return mContext;}

        virtual ~Thread(){};

    private:
        ContextPtr mContext;
        ThreadTask mTask;
    };
}

#endif //PROJECT_THREAD_HPP
