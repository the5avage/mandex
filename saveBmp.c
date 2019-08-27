#include <malloc.h>
#include <stdint.h>
#include <stdio.h>
#include "saveBmp.h"


#define SIZE_BMP_HEADER 54 // size without padding

// File header according to bmp file format
struct BmpHeader {
    char padForAlingnment[2]; //pad header size to multiple of 4 bytes
    // BMP Fileheader
    char headerField[2];
    uint32_t fileSize;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offsetPixelArray;
    // BITMAPINFOHEADER
    uint32_t headerSize;
    int32_t bmpWidth;
    int32_t bmpHeight;
    uint16_t numColorPlanes;
    uint16_t bitsPerPixel;
    uint32_t compressionMethod;
    uint32_t bmpPixelArraySize;
    uint32_t horizontalResolution;      // pixel / metre
    uint32_t verticalResolution;
    uint32_t numColorsInPalette;
    uint32_t numImportantColors;
};

// for byte access
typedef union {
    uint32_t four;
    struct {
        uint8_t alpha;
        uint8_t blue;
        uint8_t green;
        uint8_t red;
    };
} Pixel;

void writeBMP_header(struct BmpHeader* bmpHeader,
                     int32_t width,
                     int32_t height,
                     uint32_t sizePixelArray)
{
    // BMP Fileheader
    (*bmpHeader).headerField[0]         = 'B';
    (*bmpHeader).headerField[1]         = 'M';
    (*bmpHeader).fileSize               = SIZE_BMP_HEADER + sizePixelArray;
    (*bmpHeader).reserved1              = 0x6552;         // 'Re'
    (*bmpHeader).reserved2              = 0x656E;         // 'ne'
    (*bmpHeader).offsetPixelArray       = SIZE_BMP_HEADER;
    //BITMAPINFOHEADER
    (*bmpHeader).headerSize             = 40;
    (*bmpHeader).bmpWidth               = width;
    (*bmpHeader).bmpHeight              = height;
    (*bmpHeader).numColorPlanes         = 1;
    (*bmpHeader).bitsPerPixel           = 24;
    (*bmpHeader).compressionMethod      = 0;
    (*bmpHeader).bmpPixelArraySize      = sizePixelArray;
    (*bmpHeader).horizontalResolution   = 2835;      // pixel / metre
    (*bmpHeader).verticalResolution     = 2835;
    (*bmpHeader).numColorsInPalette     = 0;
    (*bmpHeader).numImportantColors     = 0;
}

void writeBMP_body(const Pixel* source,
                   uint8_t* target,
                   int32_t width,
                   int32_t height,
                   uint32_t paddedWidth)
{
    for (int32_t row = 0; row < height; row++) {
        for (int32_t column = 0; column < width; column++) {
            target[row * paddedWidth + column * 3]     = source[row * width + column].blue;
            target[row * paddedWidth + column * 3 + 1] = source[row * width + column].green;
            target[row * paddedWidth + column * 3 + 2] = source[row * width + column].red;
        }
    }
}

static inline uint32_t roundMultipleOf4(uint32_t x)
{
    return (x * 3 + 3) &~ 3;
}

uint8_t* createBMP(const uint32_t* pixels, int32_t width, int32_t height, uint32_t* bmpSize)
{
    if (width <= 0 || height == 0 || pixels == NULL)
        return NULL;

    int y_direction = 1;
    if (height < 0) {
        height *= -1;
        y_direction = -1;
    }

    // calculate size width padding
    uint32_t paddedWidth = roundMultipleOf4(width);
    uint32_t sizePixelArray = paddedWidth * height;
    *bmpSize = SIZE_BMP_HEADER + sizePixelArray;

    uint8_t* bmpBuffer = malloc(sizeof(struct BmpHeader) + sizePixelArray);
    if (bmpBuffer == NULL)
        return NULL;

    writeBMP_header((struct BmpHeader*)bmpBuffer, width, height * y_direction, sizePixelArray);

    writeBMP_body((Pixel*)pixels, bmpBuffer + sizeof(struct BmpHeader), width, height, paddedWidth);

    return bmpBuffer;
}


int saveBMP(const char* filename, const uint32_t* pixels, int32_t width, int32_t height)
{
    FILE* f = fopen(filename, "wb");
    if ( f == NULL )
        return -1;
    uint32_t bmpSize;
    uint8_t* bmpBuffer = createBMP(pixels, width, height, &bmpSize);
    if (bmpBuffer == NULL) {
        fclose(f);
        return -2;
    }
    if (fwrite(bmpBuffer + 2, 1, bmpSize, f) != bmpSize) {  // BMP Data starts with offset two because of alignment
        fclose(f);
        free(bmpBuffer);
        return -3;
    }
    fclose(f);
    free(bmpBuffer);
    return 0;
}
