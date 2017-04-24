//
// Created by dapl on 2017-04-21.
//

#include <gtest/gtest.h>
#include <MsbLsb.hpp>

using namespace PiOS;

class MsbLsbTest : public ::testing::Test {
public:
    int msb(uint32_t value) {
        unsigned int idxPos = 0;
        while (value >>= 1) idxPos++;
        return idxPos;
    }
};

TEST_F(MsbLsbTest, MSBForInteger) {
    MsbLsb calculator;

    ASSERT_EQ(calculator.calculateMSB(0), -1);

    uint32_t value = 1;
    for (; value < 256 - 4; value += 4) {
        ASSERT_EQ(calculator.calculateMSB(value), msb(value));
    }

    for (; value < 0xffff - 200; value += 200) {
        ASSERT_EQ(calculator.calculateMSB(value), msb(value));
    }

    for (; value < 0xffffffff - 1000; value += 1000) {
        ASSERT_EQ(calculator.calculateMSB(value), msb(value));
    }
}

TEST_F(MsbLsbTest, MSBForBitset) {
    MsbLsb calculator;

    std::bitset<32> bitset;
    ASSERT_EQ(calculator.calculateMSB(bitset), -1);

    unsigned int value = 0;
    for (unsigned int idx = 0; idx < 32; ++idx) {
        value |= 1 << idx;
        ASSERT_EQ(calculator.calculateMSB(value), idx);
    }
}

TEST_F(MsbLsbTest, LSBForBitset) {
    MsbLsb calculator;

    std::bitset<32> bitset;
    ASSERT_EQ(calculator.calculateMSB(bitset), -1);

    for (unsigned int idx = 0; idx < 32; ++idx) {
        std::bitset<32> value2Check;
        value2Check.set(idx);
        ASSERT_EQ(calculator.calculateLSB(value2Check), idx);
    }
}

TEST_F(MsbLsbTest, LSBForInteger) {
    MsbLsb calculator;

    ASSERT_EQ(calculator.calculateMSB(0), -1);

    for (unsigned int idx = 0; idx < 32; ++idx) {
        int value = 1 << idx;
        ASSERT_EQ(calculator.calculateLSB(value), idx);
    }
}