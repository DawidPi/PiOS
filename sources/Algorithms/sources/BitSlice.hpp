//
// Created by dapl on 2017-04-21.
//

#ifndef PROJECT_BITSLICECALCULATOR_HPP
#define PROJECT_BITSLICECALCULATOR_HPP

#include <cstdint>
#include <bitset>
#include <climits>

namespace PiOS {
    /*!
     * \brief Calculates the value of the slice of bits of the given value.
     */
    class BitSlice {
    public:
        using MaxType = std::size_t;
        static constexpr std::size_t maxBits = CHAR_BIT * sizeof(MaxType);
        /*!
         * \brief Calculates value of the bits slice.
         *
         * @param value Value, which slice [begin, end) will be calculated.
         * @param begin Starting index of the slice. Must be <= 32 and <= than end.
         * @param end Ending index of the slice. Must be <= 32 and >= begin.
         * @return Calculated value
         */
        std::size_t calculate(std::size_t value, unsigned int begin, unsigned int end);

        /*!
         * \brief Calculates value of the bits slice.
         *
         * @param value Bitset value, which slice [begin, end) will be calculated.
         * @param begin Starting index of the slice. Must be < 32 and <= than end.
         * @param end Ending index of the slice. Must be <= 32 and >= begin.
         * @return Calculated value
         */
        template<std::size_t BITS>
        std::bitset<BITS> calculate(std::bitset<BITS> value, unsigned int begin, unsigned int end) {
            return calculate(static_cast<std::size_t>(value.to_ullong()), begin, end);
        }
    };
}


#endif //PROJECT_BITSLICECALCULATOR_HPP
