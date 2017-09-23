//
// Created by dawid on 25.06.17.
//

#include "Register.hpp"

using UART_BASE = RegisterBase<0x101f1000>;
using UART0_DR = Register<UART_BASE, 0x0>;
using UART0_FR = Register<UART_BASE, 0x18>;



extern "C" {

void uart_send(unsigned int character) {
    UART0_DR::value(character);
}

unsigned int uart_read() {
    return UART0_DR::value();
}

bool uart_dataToRead() {
    bool receiveEmpty = UART0_FR::value() & 1 << 4;
    return !receiveEmpty;
}

void uart_flush() {}

}
