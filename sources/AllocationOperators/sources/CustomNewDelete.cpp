//
// Created by dapl on 2017-05-05.
//

#include <cstddef>
#include <new>
#include <Pi.hpp>
#include <cassert>
#include <iostream>

void *operator new(std::size_t sizeInBytes, const std::nothrow_t &) noexcept {
    auto pi = PiOS::PiOSHolder::getInstance();
    assert(pi != nullptr);

    return pi->allocator().allocate(sizeInBytes).begin();
}

void *operator new(std::size_t sizeInBytes) {
    auto result = operator new(sizeInBytes, std::nothrow);
    if (result == nullptr)
        throw std::bad_alloc();

    return result;
}

void operator delete(void *areaToDeallocate, const std::nothrow_t &) noexcept {
    auto pi = PiOS::PiOSHolder::getInstance();
    assert(pi != nullptr);

    pi->allocator().deallocate(areaToDeallocate);
}

void operator delete(void *areaToDeallocate) noexcept {
    operator delete(areaToDeallocate, std::nothrow);
}


void operator delete(void *areaToDeallocate, unsigned int, const std::nothrow_t &) noexcept {
    operator delete(areaToDeallocate, std::nothrow);
}

// sized deallocaiton. Should use normal deallocation in our case
void operator delete(void *areaToDeallocate, unsigned int size) {
    operator delete(areaToDeallocate, size, std::nothrow);
}
