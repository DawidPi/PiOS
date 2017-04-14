//
// Created by dapl on 2017-04-14.
//

#include <gtest/gtest.h>
#include <ListEDF.hpp>

using namespace PiOS;

class ListEDFTest : public ::testing::Test {
public:
    ListEDFTest() : mEDF([this]() { this->mErrorCalled = true; }) {

    }

    bool mErrorCalled = false;
    ListEDF mEDF;
};

TEST_F(ListEDFTest, BasicTests) {
    auto task = mEDF.fetchNextTask();


}