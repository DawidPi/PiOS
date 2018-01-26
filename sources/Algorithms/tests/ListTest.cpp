//
// Created by dawid on 1/25/18.
//

#include <gtest/gtest.h>
#include <numeric>
#include <List.hpp>

using namespace PiOS;

class ListTest : public ::testing::Test {

};

TEST_F(ListTest, creationTest) {
    {
        List<int> list;

        ASSERT_EQ(list.size(), 0);
        ASSERT_EQ(list.begin(), list.end());
        auto sum = std::accumulate(list.begin(), list.end(), 0);
        ASSERT_EQ(sum, 0);
    }

    {
        List<int> list(10);

        ASSERT_EQ(list.size(), 10);
        ASSERT_NE(list.begin(), list.end());
        ASSERT_EQ(std::distance(list.begin(), list.end()), 10);
        auto sum = std::accumulate(list.begin(), list.end(), 0);
        ASSERT_EQ(sum, 0);
        ASSERT_TRUE(std::all_of(list.begin(), list.end(), [](auto &elem) { return elem == 0; }));
    }

    {
        List<int> list(0, 1);

        ASSERT_EQ(list.size(), 0);
        ASSERT_EQ(list.begin(), list.end());
        ASSERT_EQ(std::distance(list.begin(), list.end()), 0);
        auto sum = std::accumulate(list.begin(), list.end(), 0);
        ASSERT_EQ(sum, 0);
    }

    {
        List<int> list(20, 1);

        ASSERT_EQ(list.size(), 20);
        ASSERT_NE(list.begin(), list.end());
        ASSERT_EQ(std::distance(list.begin(), list.end()), 20);
        auto sum = std::accumulate(list.begin(), list.end(), 0);
        ASSERT_EQ(sum, 20);
        ASSERT_TRUE(std::all_of(list.begin(), list.end(), [](auto &elem) { return elem == 1; }));
    }

    {
        List<int> list{20, 1};

        ASSERT_EQ(list.size(), 2);
        ASSERT_NE(list.begin(), list.end());
        ASSERT_EQ(std::distance(list.begin(), list.end()), 2);
        auto sum = std::accumulate(list.begin(), list.end(), 0);
        ASSERT_EQ(sum, 21);
        auto element = list.begin();
        ASSERT_EQ(*element, 20);
        ASSERT_EQ(*++element, 1);
    }

    {
        List<int> list{20, 1, 2, 10, 15};

        ASSERT_EQ(list.size(), 5);
        ASSERT_NE(list.begin(), list.end());
        ASSERT_EQ(std::distance(list.begin(), list.end()), 5);
        auto sum = std::accumulate(list.begin(), list.end(), 0);
        ASSERT_EQ(sum, 48);
        auto element = list.begin();
        ASSERT_EQ(*element, 20);
        ASSERT_EQ(*++element, 1);
        ASSERT_EQ(*++element, 2);
        ASSERT_EQ(*++element, 10);
        ASSERT_EQ(*++element, 15);
    }

}


TEST_F(ListTest, copyTest) {
    List<int> list{20, 1, 2, 10, 15};
    List<int> cpList(list);

    ASSERT_EQ(cpList.size(), 5);
    ASSERT_NE(cpList.begin(), cpList.end());
    ASSERT_EQ(std::distance(cpList.begin(), cpList.end()), 5);
    auto sum = std::accumulate(cpList.begin(), cpList.end(), 0);
    ASSERT_EQ(sum, 48);
    auto element = cpList.begin();
    ASSERT_EQ(*element, 20);
    ASSERT_EQ(*++element, 1);
    ASSERT_EQ(*++element, 2);
    ASSERT_EQ(*++element, 10);
    ASSERT_EQ(*++element, 15);

}

TEST_F(ListTest, insertionAndErase) {
    List<int> list;
    list.push_front(1);
    list.push_front(2);
    list.push_front(3);
    list.push_front(4);

    ASSERT_EQ(list.size(), 4);
    auto current = list.begin();
    ASSERT_EQ(*current, 4);
    ASSERT_EQ(*++current, 3);
    ASSERT_EQ(*++current, 2);
    ASSERT_EQ(*++current, 1);

    list.erase(current);
    ASSERT_EQ(list.size(), 3);
    current = list.begin();
    ASSERT_EQ(*current, 4);
    ASSERT_EQ(*++current, 3);
    ASSERT_EQ(*++current, 2);
}

TEST_F(ListTest, clear) {

    List<int> list;
    list.clear();

    ASSERT_EQ(list.size(), 0);
    ASSERT_EQ(list.begin(), list.end());

    list.insert(list.begin(), 5, 1);
    ASSERT_EQ(list.size(), 5);
    ASSERT_NE(list.begin(), list.end());

    list.clear();
    ASSERT_EQ(list.size(), 0);
    ASSERT_EQ(list.begin(), list.end());

}

TEST_F(ListTest, emptyTest) {
    List<int> list;

    ASSERT_EQ(list.size(), 0);
    ASSERT_TRUE(list.empty());

    list.insert(list.begin(), 0);
    ASSERT_EQ(list.size(), 1);
    ASSERT_FALSE(list.empty());
}