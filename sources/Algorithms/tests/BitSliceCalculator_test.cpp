//
// Created by dapl on 2017-04-21.
//

#include <gtest/gtest.h>
#include <BitSlice.hpp>

using namespace PiOS;

class BitSliceTest : public ::testing::Test {
public:

};

TEST_F(BitSliceTest, validOutputTest) {
    BitSlice calculator;

    for (auto value : {(uint32_t) 0, (uint32_t) 47, std::numeric_limits<uint32_t>::max()}) {
        auto output = calculator.calculate(value, 0, 32);
        ASSERT_EQ(output, value);
    }


    uint32_t value = 0b111u << 11u;
    std::vector<std::pair<int, int>> dataset{{1, 12},
                                             {3, 13},
                                             {7, 14},
                                             {7, 15}};

    for (std::pair<int, int> pair : dataset) {
        ASSERT_EQ(calculator.calculate(value, 11, pair.second), pair.first);
    }

    for (std::pair<int, int> pair : dataset) {
        ASSERT_EQ(calculator.calculate(std::bitset<32>(value), 11, pair.second), std::bitset<32>(pair.first));
    }
}

TEST_F(BitSliceTest, cornerCaseTests) {
    BitSlice calculator;

    uint32_t value = 0b111u;
    std::bitset<32> bitsetValue(value);
    ASSERT_EQ(calculator.calculate(bitsetValue, 0, 0), 0);
    ASSERT_EQ(calculator.calculate(bitsetValue, 0, 1), 1u);
    ASSERT_EQ(calculator.calculate(bitsetValue, 0, 2), 0b11u);
    ASSERT_EQ(calculator.calculate(bitsetValue, 0, 3), 0b111u);

    bitsetValue.set(31);
    ASSERT_EQ(calculator.calculate(bitsetValue, 31, 32), 1u);
}