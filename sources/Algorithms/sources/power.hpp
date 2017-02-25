//
// Created by dawid on 25.02.17.
//

#ifndef PROJECT_POWER_HPP
#define PROJECT_POWER_HPP

#include <cstddef>

namespace PiOS {

    std::size_t constexpr pow(std::size_t base, unsigned int exponent) {
        return exponent == 0 ? 1 : base * pow(base, exponent - 1);
    }

}

#endif //PROJECT_POWER_HPP
