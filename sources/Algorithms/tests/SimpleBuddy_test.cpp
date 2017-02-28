//
// Created by dawid on 28.02.17.
//

#include <gtest/gtest.h>
#include "../sources/SimpleBuddy.hpp"

using namespace PiOS;

class SimpleBuddyTest : public ::testing::Test {
public:
};


TEST_F(SimpleBuddyTest, SimpleBuddy) {

    //const size_t spaceSize =

    const size_t elementsInTree = pow(2, 5) - 1;
    size_t *elements = static_cast<size_t *>(malloc(sizeof(size_t) * elementsInTree));
    FixedSizeBinaryTree<size_t> binaryTree(elements, elementsInTree);

    //SimpleBuddy buddy(std::move(binaryTree), );
}