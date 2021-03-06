//
// Created by dapl on 2017-04-21.
//

#include <limits>
#include <cassert>
#include "BitSlice.hpp"
#include "MsbLsb.hpp"

std::size_t PiOS::BitSlice::calculate(std::size_t value, unsigned int begin, unsigned int end) {
    assert(begin <= end);
    assert(begin <= maxBits);
    assert(end <= maxBits);

    std::bitset<maxBits> allBitsSet(std::numeric_limits<std::size_t>::max());
    std::bitset<maxBits> mask = (allBitsSet >> (maxBits - end)) ^(allBitsSet >> (maxBits - begin));

    std::bitset<maxBits> bitsetValue(value);
    auto maskedValue = bitsetValue & mask;
    auto result = maskedValue >> begin;

    return static_cast<std::size_t>(result.to_ullong());
}
