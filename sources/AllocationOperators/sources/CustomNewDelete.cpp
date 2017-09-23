//
// Created by dapl on 2017-05-05.
//

#include <cstddef>
#include <new>
#include <Pi.hpp>
#include <cassert>

extern char *__current_static_heap;
extern char __static_heap_end__;
extern char __static_heap_start__;

extern "C" void *__stack_allocate(std::size_t size) {
    assert(__current_static_heap != nullptr);
    assert(__current_static_heap + size < &__static_heap_end__);

    void *result = __current_static_heap;
    __current_static_heap += size;

    return result;
}

/*!
 * \brief Operator new, that uses PiOSHolder monostate to allocate the memory.
 * Implementation does not throw, when allocation fails.
 *
 * \param sizeInBytes Size to allocate
 * \return Pointer to the beginning of the allocated block. Returns nullptr, when allocation fails.
 */
void *operator new(std::size_t sizeInBytes, const std::nothrow_t &) noexcept {
    auto pi = PiOS::PiOSHolder::getInstance();
    if (pi == nullptr)
        return __stack_allocate(sizeInBytes);

    return pi->allocator().allocate(sizeInBytes).begin();
}


/*!
 * \brief Operator new, that uses PiOSHolder monostate to allocate the memory.
 * Implementation does not throw, when allocation fails. Exists, so that std containers works fine. Should be removed later.
 *
 * \param sizeInBytes Size to allocate
 * \return Pointer to the beginning of the allocated block. Returns nullptr, when allocation fails.
 */
void *operator new(std::size_t sizeInBytes) {
    return operator new(sizeInBytes, std::nothrow);
}

/*!
 * \brief Operator delete, that uses PiOSHolder monostate to deallocate the memory.
 * Implementation does not throw, when deallocation fails.
 */
void operator delete(void *areaToDeallocate, const std::nothrow_t &) noexcept {
    auto pi = PiOS::PiOSHolder::getInstance();
    if (pi == nullptr) {
        return;
    }

    pi->allocator().deallocate(static_cast<char*>(areaToDeallocate));
}

/*!
 * \brief Operator delete, that uses PiOSHolder monostate to deallocate the memory.
 * Implementation also does not throw, but should be used to deallocate the memory which was
 * allocated without nothrow tag.
 */
void operator delete(void *areaToDeallocate) noexcept {
    operator delete(areaToDeallocate, std::nothrow);
}

/*!
 * \brief Operator delete for sized deallocation. Implementation has no advantage of using
 * this kind of delete over not sized deallocation. Implementation does not throw
 */
void operator delete(void *areaToDeallocate, std::size_t, const std::nothrow_t &) noexcept {
    operator delete(areaToDeallocate, std::nothrow);
}

/*!
 * \brief Operator delete for sized deallocation. Implementation has no advantage of using
 * this kind of delete over not sized deallocation. Implementation does not throw, but should be
 * used with new without nothrow tag.
 */
void operator delete(void *areaToDeallocate, std::size_t size) {
    operator delete(areaToDeallocate, size, std::nothrow);
}
