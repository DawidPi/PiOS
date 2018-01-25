//
// Created by dawid on 8/29/17.
//

#include "TLSFExternal.hpp"

extern"C"{

size_t init_memory_pool(size_t, void *);

void *tlsf_malloc(size_t size);
void tlsf_free(void *ptr);
void destroy_memory_pool(void *ptr);

}

TLSFExternal::TLSFExternal(void *memoryBegin, void *memoryEnd) : pool(memoryBegin) {
    std::size_t memorySize = reinterpret_cast<char*>(memoryEnd) - reinterpret_cast<char*>(memoryBegin);
    init_memory_pool(memorySize, memoryBegin);
}

PiOS::MemorySpace TLSFExternal::allocate(std::size_t size) {
    return PiOS::MemorySpace(tlsf_malloc(size), nullptr);
}

void TLSFExternal::deallocate(void *spaceBegin) {
    tlsf_free(spaceBegin);
}

TLSFExternal::~TLSFExternal() {
    destroy_memory_pool(pool);
}
