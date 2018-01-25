//
// Created by dawid on 8/29/17.
//

#ifndef PROJECT_TLSFEXTERNAL_HPP
#define PROJECT_TLSFEXTERNAL_HPP


#include <DynamicAllocator.hpp>

class TLSFExternal : public PiOS::DynamicAllocator{
public:
    TLSFExternal(void *memoryBegin, void *memoryEnd);
    TLSFExternal(const TLSFExternal&) = delete;
    TLSFExternal& operator=(const TLSFExternal&)=delete;

    PiOS::MemorySpace allocate(std::size_t size) override;

    void deallocate(void *spaceBegin) override;

    ~TLSFExternal();

private:
    void* pool;
};


#endif //PROJECT_TLSFEXTERNAL_HPP
