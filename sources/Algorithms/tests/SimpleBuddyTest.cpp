//
// Created by dawid on 02.03.17.
//

#include <gtest/gtest.h>
#include <SimpleBuddyFactory.hpp>

using namespace PiOS;
using namespace std::literals;

class SimpleBuddyTest : public ::testing::Test {
public:
    const std::size_t binaryTreeElements = PiOS::pow(2, 4) - 1;
    const std::size_t memorySize = 1024;
    void* managedMemory = operator new (memorySize);
    SimpleBuddyTest() :
    mSimpleBuddy{SimpleBuddyFactory().create(
            new std::size_t[binaryTreeElements], binaryTreeElements,
            managedMemory, memorySize)} {}

    SimpleBuddyTest(const SimpleBuddyTest& rhs) = delete;
    SimpleBuddyTest& operator=(const SimpleBuddyTest& rhs) = delete;

    SimpleBuddy mSimpleBuddy;
    std::array<void *, 4> mAllocatedPtrs{{nullptr, nullptr, nullptr, nullptr}};
    std::array<std::size_t, 4> mAllocationSizes{{1, 128, 256, 512}};

    void allocateAllSpace() {
        for (std::size_t elementIdx = 0; elementIdx < mAllocationSizes.size(); ++elementIdx) {
            const auto allocationSize = mAllocationSizes[elementIdx];
            mAllocatedPtrs[elementIdx] = mSimpleBuddy.allocate(allocationSize).begin();
        }
    }
};


TEST_F(SimpleBuddyTest, SimpleBuddy_allocation) {
    const int expectedPageSize = 128; // 1024/(2^(4-1)) = 128

    //allocate 2 smallest elements 128 bytes
    auto allocatedSpace = mSimpleBuddy.allocate(mAllocationSizes[0]);
    ASSERT_EQ(allocatedSpace.begin(), managedMemory);
    auto endOfAllocatedMemory = static_cast<char*>(managedMemory) + expectedPageSize;
    ASSERT_EQ(allocatedSpace.end(), endOfAllocatedMemory);
    mAllocatedPtrs[0] = allocatedSpace.begin();

    //second 128 byte
    allocatedSpace = mSimpleBuddy.allocate(mAllocationSizes[1]);
    auto beginningOfAllocatedMemory = endOfAllocatedMemory;
    ASSERT_EQ(allocatedSpace.begin(), beginningOfAllocatedMemory);
    endOfAllocatedMemory = beginningOfAllocatedMemory + expectedPageSize;
    ASSERT_EQ(allocatedSpace.end(), endOfAllocatedMemory);
    mAllocatedPtrs[1] = allocatedSpace.begin();

    // 1024 - 256 bytes left allocate next 256 bytes
    allocatedSpace = mSimpleBuddy.allocate(mAllocationSizes[2]);
    beginningOfAllocatedMemory = endOfAllocatedMemory;
    ASSERT_EQ(allocatedSpace.begin(), beginningOfAllocatedMemory);
    endOfAllocatedMemory = beginningOfAllocatedMemory + 2*expectedPageSize;
    ASSERT_EQ(allocatedSpace.end(), endOfAllocatedMemory);
    mAllocatedPtrs[2] = allocatedSpace.begin();

    // 512 bytes left allocate 512 bytes
    allocatedSpace = mSimpleBuddy.allocate(mAllocationSizes[3]);
    beginningOfAllocatedMemory = endOfAllocatedMemory;
    ASSERT_EQ(allocatedSpace.begin(), beginningOfAllocatedMemory);
    endOfAllocatedMemory = beginningOfAllocatedMemory + 4*expectedPageSize;
    ASSERT_EQ(allocatedSpace.end(), endOfAllocatedMemory);
    mAllocatedPtrs[3] = allocatedSpace.begin();

    //no more space to allocate nullptr should be returned
    allocatedSpace = mSimpleBuddy.allocate(1024);
    ASSERT_EQ(allocatedSpace.begin(), nullptr);
    ASSERT_EQ(allocatedSpace.end(), nullptr);
}

TEST_F(SimpleBuddyTest, SimpleBuddy_deallocation) {
    //all memory is allocated, because of allocation test.
    //we can check whether memory was well deallocated by
    //deallocating and rellocating the same space once again
    allocateAllSpace();

    mSimpleBuddy.deallocate(nullptr);

    for (std::size_t allocationNo = 0; allocationNo < mAllocatedPtrs.size(); ++allocationNo) {
        mSimpleBuddy.deallocate(mAllocatedPtrs[allocationNo]);
        auto allocatedSpace = mSimpleBuddy.allocate(mAllocationSizes[allocationNo]);
        EXPECT_EQ(allocatedSpace.begin(), mAllocatedPtrs[allocationNo]);
    }
    ASSERT_EQ(mSimpleBuddy.allocate(1).begin(), nullptr);

    //only crash test
    for (std::size_t allocationNo = 0; allocationNo < mAllocatedPtrs.size(); ++allocationNo) {
        mSimpleBuddy.deallocate(mAllocatedPtrs[allocationNo]);
    }

    ASSERT_EQ(mSimpleBuddy.allocate(1024).begin(), managedMemory);
    mSimpleBuddy.deallocate(managedMemory);

}