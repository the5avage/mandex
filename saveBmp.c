#include <malloc.h>
#include <stdint.h>
#include <stdio.h>
#include "saveBmp.h"

// The file header
typedef struct {
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
} TBmpHeader;

// for byte access
typedef union {
    uint32_t four;
    struct {
        uint8_t alpha;
        uint8_t blue;
        uint8_t green;
        uint8_t red;
    };
} TPixel;

uint8_t* createBMP(const uint32_t* pixels, int32_t width, int32_t height, uint32_t* bmpSize)
{
    if (width <= 0 || pixels == NULL)
        return NULL;

    // invert y if height < 0
    int y_direction = 1;
    if (height < 0) {
        height *= -1;
        y_direction = -1;
    }

    // calculate size width padding
    uint32_t byteWidth = (width * 3 + 3) &~ 3;       // round to multiple of 4
    uint32_t sizePixelArray = byteWidth * height;
    *bmpSize = 54 + sizePixelArray;

    // pointer to stored bitmap
    uint8_t* bmpBuffer = malloc(sizeof(TBmpHeader) + sizePixelArray);
    if (bmpBuffer == NULL)
        return NULL;
    // pointers for easy access
    TBmpHeader* restrict bmpHeader         = (TBmpHeader*) bmpBuffer;              // for header access in struct style
    uint8_t* restrict bmpPixel             = bmpBuffer + sizeof(TBmpHeader);       // for target pixel access (no struct because of padding)
    const TPixel* restrict sourcePixel     = (const TPixel* restrict) pixels;      // for source pixel access in struct style

    //write header
    // BMP Fileheader
    (*bmpHeader).headerField[0]         = 'B';
    (*bmpHeader).headerField[1]         = 'M';
    (*bmpHeader).fileSize               = *bmpSize;
    (*bmpHeader).reserved1              = 0x6552;                           // 'Re'
    (*bmpHeader).reserved2              = 0x656E;                           // 'ne'
    (*bmpHeader).offsetPixelArray       = 54;
    //BITMAPINFOHEADER
    (*bmpHeader).headerSize             = 40;
    (*bmpHeader).bmpWidth               = width;
    (*bmpHeader).bmpHeight              = height * y_direction;
    (*bmpHeader).numColorPlanes         = 1;
    (*bmpHeader).bitsPerPixel           = 24;
    (*bmpHeader).compressionMethod      = 0;
    (*bmpHeader).bmpPixelArraySize      = sizePixelArray;
    (*bmpHeader).horizontalResolution   = 2835;      // pixel / metre
    (*bmpHeader).verticalResolution     = 2835;
    (*bmpHeader).numColorsInPalette     = 0;
    (*bmpHeader).numImportantColors     = 0;

    // write pixel array
    int row, column;
    for (row = 0; row < height; row++) {
        for (column = 0; column < width; column++) {
            bmpPixel[row * byteWidth + column * 3]     = sourcePixel[row * width + column].blue;
            bmpPixel[row * byteWidth + column * 3 + 1] = sourcePixel[row * width + column].green;
            bmpPixel[row * byteWidth + column * 3 + 2] = sourcePixel[row * width + column].red;
        }
    }
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
        return -2;     // invalid parameters or error in malloc
    }
    if (fwrite(bmpBuffer + 2, 1, bmpSize, f) != bmpSize) {  // BMP Data starts with offset two because of alignment
        fclose(f);
        free(bmpBuffer);
        return -3;     // error in writing the file
    }
    fclose(f);
    free(bmpBuffer);
    return 0;
}
