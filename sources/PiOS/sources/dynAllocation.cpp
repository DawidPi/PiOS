//
// Created by dapl on 2017-04-27.
//

#include <cstddef>
#include <iostream>

extern "C" {
void *__wrap_malloc(std::size_t size);
void __wrap_free(void *);
}

void *__wrap_malloc(std::size_t size) {
    return nullptr;
}

void __wrap_free(void *) {

}
