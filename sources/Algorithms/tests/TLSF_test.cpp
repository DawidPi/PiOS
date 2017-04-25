//
// Created by dawid on 23.04.17.
//

#include <gtest/gtest.h>
#include <TLSF.hpp>

using namespace PiOS;

class TLSFTest : public ::testing::Test {

};

TEST_F(TLSFTest, impossibleAllocationsTest) {
    constexpr auto bufferSize = PiOS::pow(1024, 2);
    char *buffer = static_cast<char *>(::operator new(bufferSize));

    TLSF<> tlsf(MemorySpace(buffer, buffer + bufferSize));
    auto space = tlsf.allocate(PiOS::pow(1024, 3));

    ASSERT_EQ(space.begin(), nullptr);
    ASSERT_EQ(space.end(), nullptr);
    ASSERT_EQ(space.size(), 0);

    space = tlsf.allocate(0);
    ASSERT_EQ(space.begin(), nullptr);
    ASSERT_EQ(space.end(), nullptr);
    ASSERT_EQ(space.size(), 0);
}

TEST_F(TLSFTest, regularAllocationsTest) {
    constexpr auto bufferSize = PiOS::pow(1024, 2);
    char *buffer = static_cast<char *>(::operator new(bufferSize));

    TLSF<> tlsf(MemorySpace(buffer, buffer + 1024));

    auto space = tlsf.allocate(1);
    ASSERT_NE(space.begin(), nullptr);
    ASSERT_NE(space.end(), nullptr);
    ASSERT_GE(space.size(), 1);

    space = tlsf.allocate(16);
    ASSERT_GE(space.size(), 16);

    space = tlsf.allocate(32);
    ASSERT_GE(space.size(), 32);

    space = tlsf.allocate(64 + 32 + 1);
    ASSERT_GE(space.size(), 64 + 32 + 1);

    space = tlsf.allocate(1024);
    ASSERT_GE(space.size(), 1024);
}
