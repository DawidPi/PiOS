//
// Created by dawid on 8/8/17.
//

#include <cassert>
#include <cctype>
#include "OutputEngine.hpp"

char OutputEngine::mBuff[256];

const char *OutputEngine::convert(unsigned int value, unsigned int offset) {
    auto base = static_cast<unsigned int>(mFormat);
    int n=0;
    char* buff = mBuff+offset;

    switch (mFormat){
        case Format::BIN:
            *buff++ = '0';
            *buff++ = 'b';
            break;
        case Format::OCT:
            *buff++ = '0';
            break;
        case Format::DEC:break;
        case Format::HEX:
            *buff++ = '0';
            *buff++ = 'x';
            break;
    }



    unsigned int d=1;
    while (value/d >= base)
        d*=base;
    while (d!=0) {
        int dgt = value / d;
        value%= d;
        d/=base;
        if (n || dgt>0 || d==0) {
            *buff++ = dgt+(dgt<10 ? '0' : 'a' - 10);
            ++n;
        }
    }
    *buff='\0';

    return mBuff;
}

const char *OutputEngine::convert(int value) {
    if (value<0) {
        value=-value;
        mBuff[0] = '-';
        return convert(static_cast<unsigned int>(value), 1);
    } else {
        return convert(static_cast<unsigned int>(value));
    }
}

long int OutputEngine::convert(const char *string) {
    long int result = 0;
    long int signMultiply=1;
    std::size_t currentOffset = 0;
    assert(string[0] != '\0');

    if(string[0] == '-'){
        signMultiply=-1;
        ++currentOffset;
    }

    char currentCharacter = string[currentOffset];
    while(isdigit(currentCharacter)){
        result = 10*result + currentCharacter-'0';
        currentCharacter = string[++currentOffset];
    }

    return result * signMultiply;
}
