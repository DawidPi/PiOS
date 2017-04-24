//
// Created by dapl on 2017-04-21.
//

#ifndef PROJECT_MSBLSBCALCULATOR_HPP
#define PROJECT_MSBLSBCALCULATOR_HPP

#include <cstdint>
#include <bitset>
#include <array>
#include <climits>

namespace PiOS {

    /*!
     * \brief Calculates index of MSB and LSB, that is set in the given value.
     *
     * Implementation provides possibly fast and portable implementation, but requires 256 * sizeof(int) bytes of memory
     * for lookUp table.
     */
    class MsbLsb {
        template<std::size_t SIZE>
        class MSBLookUp {
        public:
            constexpr MSBLookUp() :
                    mLookUpVals() {
                mLookUpVals[0] = -1;
                for (std::size_t idx = 1; idx < SIZE; ++idx) {
                    unsigned int msb = 0;
                    std::size_t cpIdx = idx;
                    while (cpIdx >>= 1) msb++;
                    mLookUpVals[idx] = msb;
                }
            }

            constexpr int operator[](std::size_t idx) const {
                return mLookUpVals[idx];
            }

            constexpr std::size_t size() const { return SIZE; }

        private:
            int mLookUpVals[SIZE];
        };
    public:

        /*!
         * \brief Calculates index of MSB set in given value.
         * @param value Value of which set MSB index is to be calculated.
         * @return Index of calculated MSB.
         */
        constexpr int calculateMSB(std::size_t value) const {
            unsigned int bitsMoved = 0;
            while (value >= 0xff) {
                bitsMoved += 8;
                value >>= 8;
            }

            constexpr MSBLookUp<256> msbLUT;
            return msbLUT[value] + bitsMoved;
        }

        /*!
         * \brief Calculates index of MSB set in given value.
         * @param value Bitset of which set MSB index is to be calculated.
         * @return Index of calculated MSB.
         */
        template<std::size_t BITS>
        constexpr int calculateMSB(std::bitset<BITS> value) const {
            static_assert(BITS <= CHAR_BIT * sizeof(std::size_t), "bitset is too big.");
            return calculateMSB(static_cast<std::size_t>(value.to_ullong()));
        }

        /*!
         * \brief Calculates index of LSB set in given value.
         * @param value Value of which set MSB index is to be calculated.
         * @return Index of calculated LSB.
         */
        constexpr int calculateLSB(std::size_t value) const { return calculateMSB(value & -value); }

        /*!
         * \brief Calculates index of LSB set in given value.
         * @param value Bitset of which set LSB index is to be calculated.
         * @return Index of calculated LSB.
         */
        template<std::size_t BITS>
        constexpr int calculateLSB(std::bitset<BITS> value) const {
            static_assert(BITS <= CHAR_BIT * sizeof(std::size_t), "bitset is too big.");
            return calculateMSB(static_cast<std::size_t>(value.to_ullong() & -value.to_ullong()));
        }
    };
}


#endif //PROJECT_MSBLSBCALCULATOR_HPP
