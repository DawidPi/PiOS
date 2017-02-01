//
// Created by dawid on 31.01.17.
//

#ifndef PROJECT_THREAD_HPP
#define PROJECT_THREAD_HPP

#include <functional>

namespace PiOS{
    class Thread{
    public:
        using ThreadTask=void (*)(void);
        using ContextPtr=void*;

        Thread()=delete;
        Thread(const Thread&)=delete;

        Thread(ThreadTask task) noexcept : mTask(task), mContext(nullptr) {}

        ContextPtr &context() noexcept { return mContext; }

        const ContextPtr &context() const noexcept { return mContext; }

        ThreadTask task() const noexcept { return mTask; }

        virtual ~Thread(){};
    private:
        ContextPtr mContext;
        ThreadTask mTask;
    };
}

#endif //PROJECT_THREAD_HPP
