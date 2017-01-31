//
// Created by dawid on 31.01.17.
//



#include <gtest/gtest.h>
#include <Thread.hpp>

class ThreadTest : public ::testing::Test
{
public:
    class TestFunctor{
    public:
        void operator()(){

        }
    };

    class TestContext{};
};



TEST_F(ThreadTest, basic_test)
{
    TestFunctor testFunctor;
    PiOS::Thread testThread(testFunctor);

    testThread.context() = nullptr;
    ASSERT_EQ(testThread.context(), nullptr);

    PiOS::Thread anotherTestThread(std::move(testFunctor));
    anotherTestThread.context() = nullptr;
    ASSERT_EQ(testThread.context(), nullptr);

    bool fakeBoolean;

    anotherTestThread.context() = &fakeBoolean;
    testThread.context() = anotherTestThread.context();

    ASSERT_EQ(testThread.context(), anotherTestThread.context());
}