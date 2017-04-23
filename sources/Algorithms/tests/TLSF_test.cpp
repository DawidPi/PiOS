//
// Created by dawid on 23.04.17.
//

#include <gtest/gtest.h>
#include <TLSF.hpp>

using namespace PiOS;

class TLSFTest : public ::testing::Test {

};

TEST_F(TLSFTest, basicTest) {
    TLSF<> tlsf(MemorySpace(nullptr, nullptr));
}