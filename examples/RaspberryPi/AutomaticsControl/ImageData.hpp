//
// Created by dawid on 8/30/17.
//

#ifndef PIVISUALISATOR_IMAGEDATA_HPP
#define PIVISUALISATOR_IMAGEDATA_HPP

#include <cstdint>
#include <array>

class ImageData {
public:
    static constexpr uint32_t IMAGE_WIDTH=256;
    static constexpr uint32_t IMAGE_HEIGHT=128;
    static constexpr uint32_t IMAGE_SIZE=IMAGE_WIDTH*IMAGE_HEIGHT/32;
    static constexpr uint32_t IMAGE_SIZE_BYTES=IMAGE_WIDTH*IMAGE_HEIGHT/8;
    static constexpr int32_t MAX_BAR_X_OFFSET = 10;
    static constexpr int32_t MIN_VALUE = -63;
    static constexpr int32_t MAX_VALUE = 64;

    void fill(bool newValue);
    void setPixel(uint32_t width, uint32_t height);
    void clearPixel(uint32_t width, uint32_t height);
    void changePixel(uint32_t width, uint32_t height);
    bool getPixel(uint32_t width, uint32_t height)const;
    void setInput(int32_t newValue);

    void initFromData(char* data);
    char* data();
private:
    //helpers
    void getOffsets(uint32_t width, uint32_t height, uint32_t &dWordOffset, uint32_t &bitInDWord) const;

    //storage
    using DataContainer = std::array<uint32_t, IMAGE_SIZE>;
    DataContainer mData;

    //fonts
    static constexpr uint32_t FONT_WIDTH = 5;
    static constexpr uint32_t FONT_HEIGHT = 7;
    using FontStore = std::array<std::array<bool, FONT_WIDTH>, FONT_HEIGHT>;

    static const FontStore one;
    static const FontStore zero;
    static const FontStore two;
    static const FontStore three;
    static const FontStore four;
    static const FontStore five;
    static const FontStore six;
    static const FontStore seven;
    static const FontStore eight;
    static const FontStore nine;
    static const std::array<FontStore, 10> mFontDigits;

    //drawing
    void verticalLine(uint32_t xOffset);
    void leftBar(int32_t upToXOffset, int32_t yStart, int32_t yEnd);
    void printValue(int32_t valueToPrint, int32_t placeToPrint);
    void printFont(int32_t xOffset, int32_t yOffset, const FontStore &font);
    void clearInputArea();
};


#endif //PIVISUALISATOR_IMAGEDATA_HPP
