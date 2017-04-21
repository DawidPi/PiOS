//
// Created by dapl on 2017-04-21.
//

#include <limits>
#include <cassert>
#include "BitSliceCalculator.hpp"
#include "MsbLsbCalculator.hpp"

uint32_t PiOS::BitSliceCalculator::calculate(uint32_t value, unsigned int begin, unsigned int end) {
    constexpr unsigned int maxBits = 32;
    assert(begin <= end);
    assert(begin <= maxBits);
    assert(end <= maxBits);

    std::bitset<maxBits> allBitsSet(std::numeric_limits<uint32_t>::max());
    std::bitset<maxBits> mask = (allBitsSet >> (maxBits - end)) ^(allBitsSet >> (maxBits - begin));

    std::bitset<maxBits> bitsetValue(value);
    auto maskedValue = bitsetValue & mask;
    MsbLsbCalculator lsbCalculator;
    auto lsbIdx = lsbCalculator.calculateLSB(maskedValue);
    auto result = maskedValue >> lsbIdx;

    return result.to_ulong();
}
