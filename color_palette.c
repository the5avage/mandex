/*  Filename:  color_palette.c
 *
 *  Author:    René Heldmaier
 *  Copyright: (c) 2019 René Heldmaier. All rights reserved.
 *             This work is licensed under the terms of the MIT license.
 */

#include <stdint.h>
#include <stdlib.h>
#include <time.h>

typedef union {
    uint32_t rgba;
    struct {
        uint8_t alpha;    // little endian
        uint8_t blue;
        uint8_t green;
        uint8_t red;
    };
} Color32;

void colorRandom(uint32_t* colors, int num_colors)
{
    srand(time(NULL));
    while (num_colors--) {
        colors[num_colors] = rand() | 0x000000FF; // Alpha is always max
    }
}

void colorSmooth(uint32_t* colors, int num_colors)
{
    srand(time(NULL));

    Color32* restrict col = (Color32*) colors;

    col[0].rgba = rand() | 0x000000FF;
    int16_t red = col[0].red;
    int16_t blue = col[0].blue;
    int16_t green = col[0].green;
    int16_t delta_r = rand() % 7;
    int16_t delta_b = rand() % 7;
    int16_t delta_g = rand() % 7;

    for (int i = 0; i < num_colors; ++i) {
        red += delta_r;
        blue += delta_b;
        green += delta_g;
        if ( red > 255 || red < 0 ) {
            delta_r *= -1;
            red += delta_r * 2;
        }
        if ( blue > 255 || blue < 0 ) {
            delta_b *= -1;
            blue += delta_b * 2;
        }
        if ( green > 255 || green < 0 ) {
            delta_g *= -1;
            green += delta_g * 2;
        }
        col[i].red = red;
        col[i].blue = blue;
        col[i].green = green;
        col[i].alpha = 255;
    }
}
