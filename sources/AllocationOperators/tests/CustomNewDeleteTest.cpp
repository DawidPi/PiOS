//
// Created by dapl on 2017-05-05.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <Pi.hpp>
#include <Mocks.hpp>
#include "../../Algorithms/sources/TLSF.hpp"

using namespace PiOS;
using ::testing::_;
using ::testing::ReturnRef;
using ::testing::Return;

void *operator ""_ptr(unsigned long long val) {
    return reinterpret_cast<void *>(val);
}

class CustomNewDeleteTest : public ::testing::Test {
public:
    CustomNewDeleteTest() {
        auto pi = static_cast<Pi *>(malloc(sizeof(Pi)));
        auto alloc = static_cast<AllocatorMock *>(malloc(sizeof(AllocatorMock)));
        auto sched = static_cast<SchedulerMock *>(malloc(sizeof(SchedulerMock)));
        new(alloc) AllocatorMock;
        new(sched) SchedulerMock;
        new(pi) Pi(*alloc, *sched);

        PiOSHolder::choosePiImplementation(pi);
    }

    ~CustomNewDeleteTest() {
        auto pi = PiOSHolder::getInstance();
        assert(pi != nullptr);

        auto &scheduler = pi->scheduler();
        auto &allocator = pi->allocator();
        scheduler.~Scheduler();
        allocator.~DynamicAllocator();
        free(&scheduler);
        free(&allocator);
        pi->~Pi();
        free(pi);
    }
};

TEST_F(CustomNewDeleteTest, basicAllocationNoThrowTests) {
    auto pi = PiOSHolder::getInstance();
    ASSERT_NE(pi, nullptr);

    AllocatorMock &allocator = dynamic_cast<AllocatorMock &>(pi->allocator());

    EXPECT_CALL(allocator, allocate(5))
            .Times(1)
            .WillOnce(Return(MemorySpace(nullptr, nullptr)));

    auto fakeAllocationResult = operator new(5, std::nothrow);
    ASSERT_EQ(fakeAllocationResult, nullptr);

    EXPECT_CALL(allocator, allocate(20)).Times(1).WillOnce(Return(MemorySpace(5_ptr, 10_ptr)));
    fakeAllocationResult = operator new(20, std::nothrow);
    ASSERT_EQ(fakeAllocationResult, 5_ptr);
}
