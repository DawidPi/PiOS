//
// Created by dapl on 2017-04-21.
//

#ifndef PROJECT_MSBLSBCALCULATOR_HPP
#define PROJECT_MSBLSBCALCULATOR_HPP

#include <cstdint>
#include <bitset>
#include <array>

namespace PiOS {
    /*!
     * \brief Calculates index of MSB and LSB, that is set in the given value.
     *
     * Implementation provides possibly fast and portable implementation, but requires 256 * sizeof(int) bytes of memory
     * for lookUp table.
     */
    class MsbLsbCalculator {
    public:
        /*!
         * \brief Calculates index of MSB set in given value.
         * @param value Value of which set MSB index is to be calculated.
         * @return Index of calculated MSB.
         */
        int calculateMSB(uint32_t value);

        /*!
         * \brief Calculates index of MSB set in given value.
         * @param value Bitset of which set MSB index is to be calculated.
         * @return Index of calculated MSB.
         */
        int calculateMSB(std::bitset<32> value) { return calculateMSB(value.to_ulong()); }

        /*!
         * \brief Calculates index of LSB set in given value.
         * @param value Value of which set MSB index is to be calculated.
         * @return Index of calculated LSB.
         */
        int calculateLSB(uint32_t value) { return calculateMSB(value & -value); }

        /*!
         * \brief Calculates index of LSB set in given value.
         * @param value Bitset of which set LSB index is to be calculated.
         * @return Index of calculated LSB.
         */
        int calculateLSB(std::bitset<32> value) { return calculateMSB(value.to_ulong() & -value.to_ulong()); }

    private:

        template<size_t SIZE>
        class MSBLookUp {
        public:
            constexpr MSBLookUp() :
                    mLookUpVals() {
                mLookUpVals[0] = -1;
                for (size_t idx = 1; idx < mLookUpVals.size(); ++idx) {
                    unsigned int msb = 0;
                    size_t cpIdx = idx;
                    while (cpIdx >>= 1) msb++;
                    mLookUpVals[idx] = msb;
                }
            }

            constexpr int operator[](size_t idx) {
                return mLookUpVals[idx];
            }

            constexpr size_t size() { return mLookUpVals.size(); }

        private:
            std::array<int, SIZE> mLookUpVals;
        };

        static MSBLookUp<256> mLookUp;
    };
}


#endif //PROJECT_MSBLSBCALCULATOR_HPP
