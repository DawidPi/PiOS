//
// Created by dawid on 1/25/18.
//

#include <gtest/gtest.h>
#include <numeric>
#include <List.hpp>

using namespace PiOS;

class ListTest : public ::testing::Test {

};

TEST_F(ListTest, creationTest){
    List<int> list;

    ASSERT_EQ(list.size(), 0);
    ASSERT_EQ(list.begin(), list.end());
    auto sum = std::accumulate(list.begin(), list.end(), 0);
    ASSERT_EQ(sum, 0);


}
