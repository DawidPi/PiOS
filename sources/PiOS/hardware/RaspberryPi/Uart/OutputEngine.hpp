//
// Created by dawid on 8/8/17.
//

#ifndef PROJECT_INT2STR_HPP
#define PROJECT_INT2STR_HPP

//thanks to Kustaa Nyholm / SpareTimeLabs

class OutputEngine {
public:
    enum class Format {BIN=2, OCT=8, DEC=10, HEX=16};
    void setFormatting(Format newFormat){ mFormat = newFormat;}
    Format getFormatting(){return mFormat;}
protected:
    const char* convert(unsigned int value, unsigned int offset=0);
    const char* convert(int value);
    long int convert(const char* string);
private:
    Format mFormat = Format::DEC;
    static char mBuff[256];
};


#endif //PROJECT_INT2STR_HPP
