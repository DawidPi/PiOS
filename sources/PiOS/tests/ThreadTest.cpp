//
// Created by dawid on 31.01.17.
//



#include <gtest/gtest.h>
#include <Thread.hpp>

class ThreadTest : public ::testing::Test
{
public:
    static void testMain() {

    }

    class TestContext{};
};


TEST_F(ThreadTest, members_test)
{
    PiOS::Thread testThread(testMain);

    ASSERT_EQ(testThread.task(), testMain);
    ASSERT_EQ(testThread.context(), nullptr);

    using ContextType=PiOS::Thread::ContextPtr;

    TestContext sampleConcreteContext;
    ContextType testContext = reinterpret_cast<ContextType>(&sampleConcreteContext);
    testThread.context() = testContext;
    ASSERT_EQ(testThread.context(), testContext);

    //check if Context is copied and not taken by reference
    ASSERT_NE(&testThread.context(), &testContext);
}