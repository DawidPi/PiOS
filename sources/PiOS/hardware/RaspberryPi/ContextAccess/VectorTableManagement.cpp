//
// Created by dawid on 8/11/17.
//

#include <cstring>
#include <Register.hpp>
#include "VectorTableManagement.hpp"


static constexpr auto VECTOR_TABLE_ADDRESS_SRC = 0x10000;
static constexpr auto VECTOR_TABLE_ADDRESS = 0x0;
static constexpr auto VECTOR_TABLE_ADDRESS_HIGH = 0xffff0000;

void VectorTable::setUp() {
    std::memcpy(reinterpret_cast<uint32_t*>(VECTOR_TABLE_ADDRESS), reinterpret_cast<uint32_t*>(VECTOR_TABLE_ADDRESS_SRC), 0x40);
    std::memcpy(reinterpret_cast<uint32_t*>(VECTOR_TABLE_ADDRESS_HIGH), reinterpret_cast<uint32_t*>(VECTOR_TABLE_ADDRESS_SRC), 0x40);
}
