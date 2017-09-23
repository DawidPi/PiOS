//
// Created by dawid on 8/24/17.
//

#ifndef PROJECT_BANKEDREGISTERS_HPP
#define PROJECT_BANKEDREGISTERS_HPP


#include <cstdint>

namespace BankedRegisters {

    std::uint32_t systemStackPointer();

    std::uint32_t systemLinkRegister();

    void systemLinkRegister(std::uint32_t newLr);

    void systemStackPointer(std::uint32_t newSP);

    void* currentStackPointer();

}


#endif //PROJECT_BANKEDREGISTERS_HPP
