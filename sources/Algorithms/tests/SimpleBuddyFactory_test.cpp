//
// Created by dawid on 02.03.17.
//

#include <gtest/gtest.h>
#include <SimpleBuddyFactory.hpp>

using namespace PiOS;
using namespace std::literals;

class SimpleBuddyFactoryTest : public ::testing::Test {
public:
};


TEST_F(SimpleBuddyFactoryTest, SimpleBuddy) {
    SimpleBuddyFactory factory;
    size_t memorySize = 1024; // 1024 bytes of memory
    size_t binaryTreeElements = PiOS::pow(2, 4) - 1; //tree has depth of 4
    const int expectedPageSize = 128; // 1024/(2^(4-1)) = 128
    size_t *memoryForBinaryTree = new size_t[binaryTreeElements];

    void *managedMemory = operator new(memorySize);
    SimpleBuddy myBuddy{factory.create(memoryForBinaryTree, binaryTreeElements, managedMemory, memorySize)};


    //allocate 2 smallest elements 128 bytes
    auto allocatedSpace = myBuddy.allocate(1);
    ASSERT_EQ(allocatedSpace.begin(), managedMemory);
    auto endOfAllocatedMemory = static_cast<char*>(managedMemory) + expectedPageSize;
    ASSERT_EQ(allocatedSpace.end(), endOfAllocatedMemory);

    //second 128 byte
    allocatedSpace = myBuddy.allocate(expectedPageSize);
    auto beginningOfAllocatedMemory = endOfAllocatedMemory;
    ASSERT_EQ(allocatedSpace.begin(), beginningOfAllocatedMemory);
    endOfAllocatedMemory = beginningOfAllocatedMemory + expectedPageSize;
    ASSERT_EQ(allocatedSpace.end(), endOfAllocatedMemory);

    // 1024 - 256 bytes left allocate next 256 bytes
    allocatedSpace = myBuddy.allocate(2*expectedPageSize);
    beginningOfAllocatedMemory = endOfAllocatedMemory;
    ASSERT_EQ(allocatedSpace.begin(), beginningOfAllocatedMemory);
    endOfAllocatedMemory = beginningOfAllocatedMemory + 2*expectedPageSize;
    ASSERT_EQ(allocatedSpace.end(), endOfAllocatedMemory);

    // 512 bytes left allocate 512 bytes
    allocatedSpace = myBuddy.allocate(4*expectedPageSize - 42);
    beginningOfAllocatedMemory = endOfAllocatedMemory;
    ASSERT_EQ(allocatedSpace.begin(), beginningOfAllocatedMemory);
    endOfAllocatedMemory = beginningOfAllocatedMemory + 4*expectedPageSize;
    ASSERT_EQ(allocatedSpace.end(), endOfAllocatedMemory);

    //no more space to allocate nullptr should be returned
    allocatedSpace = myBuddy.allocate(4*expectedPageSize - 42);
    ASSERT_EQ(allocatedSpace.begin(), nullptr);
    ASSERT_EQ(allocatedSpace.end(), nullptr);
}