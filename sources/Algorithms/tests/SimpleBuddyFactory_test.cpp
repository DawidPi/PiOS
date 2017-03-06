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


    auto allocatedSpace = myBuddy.allocate(1);
    ASSERT_EQ(allocatedSpace.begin(), managedMemory);
    ASSERT_EQ(allocatedSpace.end(), static_cast<char*>(managedMemory) + expectedPageSize);
}