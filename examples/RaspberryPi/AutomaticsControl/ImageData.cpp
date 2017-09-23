//
// Created by dawid on 8/30/17.
//

#include <limits>
#include <iostream>
#include <cassert>
#include <cstdlib>
#include "ImageData.hpp"

const std::array<ImageData::FontStore, 10> ImageData::mFontDigits = {zero, one, two, three, four, five, six, seven,
                                                                     eight, nine};
const ImageData::FontStore ImageData::nine = {
        0, 0, 0, 0, 0,
        0, 1, 1, 1, 0,
        0, 1, 0, 1, 0,
        0, 1, 1, 1, 0,
        0, 0, 0, 1, 0,
        0, 1, 1, 1, 0,
        0, 0, 0, 0, 0
};

const ImageData::FontStore ImageData::eight = {
        0, 0, 0, 0, 0,
        0, 1, 1, 1, 0,
        0, 1, 0, 1, 0,
        0, 1, 1, 1, 0,
        0, 1, 0, 1, 0,
        0, 1, 1, 1, 0,
        0, 0, 0, 0, 0
};

const ImageData::FontStore ImageData::seven = {
        0, 0, 0, 0, 0,
        0, 1, 1, 1, 0,
        0, 0, 0, 1, 0,
        0, 0, 1, 0, 0,
        0, 1, 0, 0, 0,
        0, 1, 0, 0, 0,
        0, 0, 0, 0, 0
};

const ImageData::FontStore ImageData::six = {
        0, 0, 0, 0, 0,
        0, 1, 1, 1, 0,
        0, 1, 0, 0, 0,
        0, 1, 1, 1, 0,
        0, 1, 0, 1, 0,
        0, 1, 1, 1, 0,
        0, 0, 0, 0, 0
};


const ImageData::FontStore ImageData::five = {
        0, 0, 0, 0, 0,
        0, 1, 1, 1, 0,
        0, 1, 0, 0, 0,
        0, 1, 1, 1, 0,
        0, 0, 0, 1, 0,
        0, 1, 1, 1, 0,
        0, 0, 0, 0, 0
};

const ImageData::FontStore ImageData::four = {
        0, 0, 0, 0, 0,
        0, 1, 0, 0, 0,
        0, 1, 0, 1, 0,
        0, 1, 1, 1, 0,
        0, 0, 0, 1, 0,
        0, 0, 0, 1, 0,
        0, 0, 0, 0, 0
};

const ImageData::FontStore ImageData::three = {
        0, 0, 0, 0, 0,
        0, 1, 1, 1, 0,
        0, 0, 0, 1, 0,
        0, 0, 1, 1, 0,
        0, 0, 0, 1, 0,
        0, 1, 1, 1, 0,
        0, 0, 0, 0, 0
};

const ImageData::FontStore ImageData::two = {
        0, 0, 0, 0, 0,
        0, 1, 1, 0, 0,
        0, 0, 0, 1, 0,
        0, 0, 1, 1, 0,
        0, 1, 0, 0, 0,
        0, 1, 1, 1, 0,
        0, 0, 0, 0, 0
};

const ImageData::FontStore ImageData::zero = {
        0, 0, 0, 0, 0,
        0, 1, 1, 1, 0,
        0, 1, 0, 1, 0,
        0, 1, 0, 1, 0,
        0, 1, 0, 1, 0,
        0, 1, 1, 1, 0,
        0, 0, 0, 0, 0
};

const ImageData::FontStore ImageData::one = {
        0, 0, 0, 0, 0,
        0, 0, 0, 1, 0,
        0, 0, 1, 1, 0,
        0, 1, 0, 1, 0,
        0, 0, 0, 1, 0,
        0, 0, 0, 1, 0,
        0, 0, 0, 0, 0
};

void ImageData::fill(bool newValue) {
    for (auto &elem : mData) {
        elem = std::numeric_limits<DataContainer::value_type>::max() * newValue;
    }
}

void ImageData::setPixel(uint32_t width, uint32_t height) {
    uint32_t dWordOffset;
    uint32_t bitInDWord;
    getOffsets(width, height, dWordOffset, bitInDWord);

    mData[dWordOffset] |= 0x80'00'00'00u >> bitInDWord;
}

void ImageData::clearPixel(uint32_t width, uint32_t height) {
    uint32_t dWordOffset;
    uint32_t bitInDWord;
    getOffsets(width, height, dWordOffset, bitInDWord);

    mData[dWordOffset] &= ~(0x80'00'00'00u >> bitInDWord);
}

bool ImageData::getPixel(uint32_t width, uint32_t height) const {
    uint32_t dWordOffset;
    uint32_t bitInDWord;
    getOffsets(width, height, dWordOffset, bitInDWord);

    return static_cast<bool>(mData[dWordOffset] & (0x80'00'00'00u >> bitInDWord));
}

void ImageData::getOffsets(uint32_t width, uint32_t height, uint32_t &dWordOffset, uint32_t &bitInDWord) const {
    uint32_t bitOffset = height * IMAGE_WIDTH + width;

    dWordOffset = bitOffset / 32;
    bitInDWord = bitOffset % 32;
}

void ImageData::setInput(int32_t newValue) {
    if (newValue > MAX_VALUE)
        newValue = MAX_VALUE;
    if (newValue < MIN_VALUE)
        newValue = MIN_VALUE;

    verticalLine(MAX_BAR_X_OFFSET + 1);

    clearInputArea();

    int32_t yStart = IMAGE_HEIGHT/2;
    int32_t yEnd = yStart - newValue;
    assert(yEnd >= 0);

    if (yEnd < yStart){
        auto tmpYStart = yStart;
        auto tmpYEnd = yEnd;

        std::swap(tmpYEnd, tmpYStart);
        leftBar(MAX_BAR_X_OFFSET, tmpYStart, tmpYEnd);
    }else {
        leftBar(MAX_BAR_X_OFFSET, yStart, yEnd);
    }
    printValue(newValue, yEnd);
}

void ImageData::leftBar(int32_t upToXOffset, int32_t yStart, int32_t yEnd) {
    for (int32_t xOffset = 0; xOffset < upToXOffset; ++xOffset) {
        for (int32_t yOffset = yStart; yOffset <= yEnd; ++yOffset) {
            setPixel(xOffset, yOffset);
        }
    }
}

void ImageData::verticalLine(uint32_t xOffset) {
    for (uint32_t yOffset = 0; yOffset < IMAGE_HEIGHT; ++yOffset) {
        setPixel(xOffset, yOffset);
    }
}

void ImageData::changePixel(uint32_t width, uint32_t height) {
    auto value = getPixel(width, height);
    if (value)
        clearPixel(width, height);
    else
        setPixel(width, height);
}

void ImageData::printValue(const int32_t valueToPrint, int32_t placeToPrint) {
    assert(valueToPrint <= 99 and valueToPrint >= -99);
    auto absValueToPrint = std::abs(valueToPrint);
    auto firstDigit = absValueToPrint / 10;
    auto secondDigit = absValueToPrint % 10;

    auto yOffset = placeToPrint;
    if(yOffset > IMAGE_HEIGHT/2 - FONT_HEIGHT and yOffset <= IMAGE_HEIGHT/2)
        yOffset=IMAGE_HEIGHT/2 - FONT_HEIGHT;
    else if (yOffset > IMAGE_HEIGHT/2)
        yOffset -= FONT_HEIGHT;

    printFont(0, yOffset, mFontDigits[firstDigit]);
    printFont(FONT_WIDTH, yOffset, mFontDigits[secondDigit]);
}

void ImageData::printFont(int32_t xOffset, int32_t yOffset, const FontStore &font) {
    for (int32_t currentY = 0; currentY < FONT_HEIGHT; ++currentY) {
        for (uint32_t currentX = 0; currentX < FONT_WIDTH; ++currentX) {
            if (font[currentY][currentX]) {
                changePixel(xOffset + currentX, yOffset + currentY);
            }
        }
    }
}

void ImageData::clearInputArea() {
    for(uint32_t xOffset=0; xOffset < MAX_BAR_X_OFFSET; xOffset++){
        for(uint32_t yOffset=0; yOffset < IMAGE_HEIGHT; yOffset++){
            clearPixel(xOffset, yOffset);
        }
    }
}

void ImageData::initFromData(char *data) {
    char* thisBegin= reinterpret_cast<char *>(mData.data());
    auto thisEnd=thisBegin + IMAGE_HEIGHT*IMAGE_WIDTH/8;

    for(char* currentByte=thisBegin; currentByte < thisEnd; ++currentByte){
        *data = *currentByte;
        data++;
    }
}

char *ImageData::data() {
    return reinterpret_cast<char *>(mData.data());
}
