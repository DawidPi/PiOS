//
// Created by dawid on 25.02.17.
//

#ifndef PROJECT_POWER_HPP
#define PROJECT_POWER_HPP

#include <cstddef>

namespace PiOS {

    /*!
     * \brief New implementation of pow, which can calculate base raised to the power exponent,
     * where base and exponent are unsigned integral types. Is able to compute expression
     * during compile time, when given arguments are constant expressions.
     * @param base Base of the expression.
     * @param exponent Exponent of the expression.
     * @return Base raised to the power of exponent.
     */
    constexpr std::size_t pow(std::size_t base, unsigned int exponent) {
        //todo add tests
        return exponent == 0 ? 1 : base * pow(base, exponent - 1);
    }

}

#endif //PROJECT_POWER_HPP
