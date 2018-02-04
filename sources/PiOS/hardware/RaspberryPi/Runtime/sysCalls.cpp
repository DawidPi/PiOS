//
// Created by dawid on 25.06.17.
//

#include <sched.h>
#include <cstdlib>
#include "Uart/Uart.hpp"

extern "C" {

void _exit(int n) {
    uart::cerr << "EXIT called: " << n << uart::endl;
    while (1);
}

int _lseek(int, int, int) {
    return 0;
}

int _write(int, char *ptr, int len) {
    int currentCharIdx = 0;
    for (; currentCharIdx < len and ptr[currentCharIdx] != '\0'; ++currentCharIdx) {
        uart::cout << ptr[currentCharIdx] << uart::flush;
    }

    return currentCharIdx;
}

int _open(const char *, int, ...) {
    return 0;
}

caddr_t _sbrk(int) {
    return nullptr;
}


int _kill(int, int) {
    return (0);
}

int _getpid(int) {
    return 1;
}

int _fstat(int, struct stat *) {
    return 0;
}

int _close(int) {
    return 0;
}

int _isatty(int) {
    return 1;
}

int _read(int,
          char *,
          int len) {
    return len;
}

void _fini() {

}

void __cxa_pure_virtual() {
    uart::cerr << "pure virtual call" << uart::endl;
    exit(0);
}

//for now synchronization is not needed.
void __sync_synchronize(){

}

void __cxa_guard_acquire(){

}

void __cxa_guard_release(){

}

}