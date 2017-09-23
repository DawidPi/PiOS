//
// Created by dapl on 2017-05-04.
//

#ifndef PROJECT_MOCKS_HPP
#define PROJECT_MOCKS_HPP

#include <gmock/gmock.h>
#include <Scheduler.hpp>
#include <DynamicAllocator.hpp>
#include <RealTimeTask.hpp>


class SchedulerMock : public PiOS::Scheduler {
public:
    MOCK_METHOD1(timeTick, void(
            const PiOS::Time&));

    void addRealTimeTask(PiOS::RealTimeTask *rval) {
        addRealTimeTaskProxy(rval);
    }

    MOCK_METHOD1(addRealTimeTaskProxy, void(
            PiOS::RealTimeTask *));

    MOCK_METHOD0(fetchNextTask, PiOS::Task * ());

    MOCK_METHOD0(finishPendingTask, void());

    MOCK_METHOD1(setBackgroundTask, void(PiOS::Task*));
};

class AllocatorMock : public PiOS::DynamicAllocator {
public:
    MOCK_METHOD1(allocate, PiOS::MemorySpace(std::size_t));

    MOCK_METHOD1(deallocate, void(void * ));
};

#endif //PROJECT_MOCKS_HPP
