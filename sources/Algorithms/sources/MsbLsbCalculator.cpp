//
// Created by dapl on 2017-04-21.
//

#include "MsbLsbCalculator.hpp"

using namespace PiOS;

MsbLsbCalculator::MSBLookUp<256> MsbLsbCalculator::mLookUp;

int MsbLsbCalculator::calculateMSB(uint32_t value) {
    unsigned int bitsMoved = 0;
    while (value >= 0xff) {
        bitsMoved += 8;
        value >>= 8;
    }

    return mLookUp[value] + bitsMoved;
}
