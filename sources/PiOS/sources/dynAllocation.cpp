//
// Created by dapl on 2017-04-27.
//

#include <cstddef>

void *__wrap_malloc(std::size_t size) {
    return nullptr;
}

void __wrap_free(void *) {

}