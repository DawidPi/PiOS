//
// Created by dapl on 2017-04-21.
//

#ifndef PROJECT_BITSLICECALCULATOR_HPP
#define PROJECT_BITSLICECALCULATOR_HPP

#include <cstdint>
#include <bitset>

namespace PiOS {
    /*!
     * \brief Calculates the value of the slice of bits of the given value.
     */
    class BitSliceCalculator {
    public:
        /*!
         * \brief Calculates value of the bits slice.
         *
         * @param value Value, which slice [begin, end) will be calculated.
         * @param begin Starting index of the slice. Must be <= 32 and <= than end.
         * @param end Ending index of the slice. Must be <= 32 and >= begin.
         * @return Calculated value
         */
        uint32_t calculate(uint32_t value, unsigned int begin, unsigned int end);

        /*!
         * \brief Calculates value of the bits slice.
         *
         * @param value Bitset value, which slice [begin, end) will be calculated.
         * @param begin Starting index of the slice. Must be < 32 and <= than end.
         * @param end Ending index of the slice. Must be <= 32 and >= begin.
         * @return Calculated value
         */
        std::bitset<32> calculate(std::bitset<32> value, unsigned int begin, unsigned int end) {
            return calculate(value.to_ulong(), begin, end);
        }
    };
}


#endif //PROJECT_BITSLICECALCULATOR_HPP
