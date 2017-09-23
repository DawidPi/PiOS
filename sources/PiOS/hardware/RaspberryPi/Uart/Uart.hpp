//
// Created by dawid on 26.06.17.
//

#ifndef PROJECT_PI_UART_HPP
#define PROJECT_PI_UART_HPP

#include <cctype>
#include "OutputEngine.hpp"

namespace uart {

    class OutUart : public OutputEngine {
    public:
        OutUart &operator<<(const char *string);

        OutUart &operator<<(char *string);

        OutUart &operator<<(const char character);

        OutUart &operator<<(void(*functor)(OutUart &));

        OutUart &operator<<(unsigned int value);
        OutUart &operator<<(long unsigned int value);

        OutUart &operator<<(int value);

        template <typename T>
        OutUart &operator<<(T* ptr);
    };

    void endl(OutUart &uart);

    void flush(OutUart &uart);

    void hex(OutUart &uart);
    void dec(OutUart &uart);
    void bin(OutUart &uart);
    void oct(OutUart &uart);

    extern OutUart cout;
    extern OutUart &cerr;
    extern OutUart &clog;

    class InUart : public OutputEngine {
    public:
        InUart& operator>>(char& character);
        InUart& operator>>(int& character);

        template <std::size_t length>
        InUart& operator>>(char (&string)[length]);
    };

    extern InUart cin;
}

template <std::size_t length>
uart::InUart& uart::InUart::operator >> (char (&string)[length]){
    char readCharacter;
    unsigned int offset=0;
    do{
        (*this) >> readCharacter;
        string[offset] = readCharacter;
        ++offset;
    }while(not std::isspace(readCharacter) and offset < length);

    string[offset] = '\0';
    return *this;

}

template <typename T>
uart::OutUart& uart::OutUart::operator<<(T* pointer) {
    auto previousFormattingOption = getFormatting();
    *this << uart::hex << convert(reinterpret_cast<unsigned int>(pointer));
    setFormatting(previousFormattingOption);
    return *this;
}

#endif //PROJECT_PI_UART_HPP
