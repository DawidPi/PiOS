//
// Created by dawid on 8/11/17.
//

#include <cstring>
#include <Register.hpp>
#include "VectorTableManagement.hpp"


static constexpr uint32_t* VECTOR_TABLE_ADDRESS_SRC = reinterpret_cast<uint32_t*>(0x10000);
static constexpr uint32_t* VECTOR_TABLE_ADDRESS = reinterpret_cast<uint32_t*>(0x0);
static constexpr uint32_t* VECTOR_TABLE_ADDRESS_HIGH = reinterpret_cast<uint32_t*>(0xffff0000);

void VectorTable::setUp() {
    std::memcpy(VECTOR_TABLE_ADDRESS, VECTOR_TABLE_ADDRESS_SRC, 0x40);
    std::memcpy(VECTOR_TABLE_ADDRESS_HIGH, VECTOR_TABLE_ADDRESS_SRC, 0x40);
}
