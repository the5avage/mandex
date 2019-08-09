/*  Filename:  color_palette.c
 *
 *  Author:    René Heldmaier
 *  Copyright: (c) 2019 René Heldmaier. All rights reserved.
 *             This work is licensed under the terms of the MIT license.
 */

#include <stdint.h>
#include <stdlib.h>
#include <time.h>

void colorRandom(uint32_t* colors, int num_colors)
{
    srand(time(NULL));
    while (num_colors--) {
        colors[num_colors] = rand() | 0x000000FF; // Alpha is always max
    }
}

void colorSmooth(uint32_t* colors, int num_colors)
{
}
