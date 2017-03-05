//
// Created by dawid on 28.02.17.
//

#include <gtest/gtest.h>
#include "../sources/SimpleBuddy.hpp"

using namespace PiOS;

class SimpleBuddyTest : public ::testing::Test {
public:
    size_t diff(void *end, void *begin) {
        return static_cast<char *>(end) - static_cast<char *>(begin);
    }

};


TEST_F(SimpleBuddyTest, SimpleBuddy) {
    EXPECT_TRUE(false);
//    //todo test fails
//    const size_t binaryTreeElements = PiOS::pow(2, 5) -1;
//    auto binaryTreeMemory = new size_t[binaryTreeElements];
//    FixedSizeBinaryTree<size_t> binaryTree(binaryTreeMemory, binaryTreeElements);
//
//    int memorySize = 16 * 2;
//    void* managedMemory = ::operator new(memorySize);
//    unsigned int minBlockExponent = 1;
//
//    SimpleBuddy buddy(std::move(binaryTree), memorySize, managedMemory, minBlockExponent);
//    auto smallestAllocatedSpace = buddy.allocate(1); //smallest possible allocation
//    size_t allocatedSpace = diff(smallestAllocatedSpace.end(), smallestAllocatedSpace.begin());
//    ASSERT_EQ(allocatedSpace, 2);
}