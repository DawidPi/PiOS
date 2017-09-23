//
// Created by dawid on 26.06.17.
//

#include <cassert>
#include <limits>
#include "Uart.hpp"
#include "c_uart.hpp"

uart::OutUart uart::cout;
uart::OutUart &uart::cerr = cout;
uart::OutUart &uart::clog = cout;

uart::OutUart &uart::OutUart::operator<<(const char *string) {
#ifdef LOGGING_ON
    const char *characterPointer = string;

    while (*characterPointer != '\0') {
        uart_send((unsigned int) *characterPointer);
        characterPointer++;
    }
#endif

    return *this;
}

uart::OutUart &uart::OutUart::operator<<(void (*functor)(OutUart &)) {
    functor(*this);
    return *this;
}

uart::OutUart &uart::OutUart::operator<<(const char character) {
#ifdef LOGGING_ON
    uart_send((unsigned int) character);
#endif
    return *this;
}

uart::OutUart &uart::OutUart::operator<<(char *string) {
    return *this << static_cast<const char *>(string);
}

uart::OutUart &uart::OutUart::operator<<(unsigned int value) {
    (*this) << convert(value);
    return *this;
}

uart::OutUart &uart::OutUart::operator<<(int value) {
    *this << convert(value);
    return *this;
}

uart::OutUart &uart::OutUart::operator<<(long unsigned int value) {
    return *this << static_cast<unsigned int>(value);
}

void uart::endl(uart::OutUart &uart) {
    uart << "\r\n" << flush;
}

void uart::flush(uart::OutUart &) {
    uart_flush();
}

void uart::hex(uart::OutUart &uart) {
    uart.setFormatting(OutUart::Format::HEX);
}

void uart::dec(uart::OutUart &uart) {
    uart.setFormatting(OutUart::Format::DEC);
}

void uart::bin(uart::OutUart &uart) {
    uart.setFormatting(OutUart::Format::BIN);
}

void uart::oct(uart::OutUart &uart) {
    uart.setFormatting(OutUart::Format::OCT);
}

uart::InUart uart::cin;

uart::InUart &uart::InUart::operator>>(char& character) {
    while(not uart_dataToRead()); // wait for data to become available
    character = static_cast<char>(uart_read() & std::numeric_limits<char>::max());
    uart::cout << character;
    return *this;
}

uart::InUart &uart::InUart::operator>>(int &character) {
    char buffor[256] = {'\0'};
    (*this) >> buffor;
    character = convert(buffor);
    return *this;
}
