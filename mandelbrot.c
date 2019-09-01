/*  Filename:  mandelbrot.c
 *
 *  Author:    René Heldmaier
 *  Copyright: (c) 2019 René Heldmaier. All rights reserved.
 *             This work is licensed under the terms of the MIT license.
 */

#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "mandelbrot.h"

// complex double
struct complexd {
    double re;
    double im;
};


struct MandelPoint {
    struct complexd c;
    struct complexd z;
    struct complexd z2;
    uint32_t diverged;
    uint32_t iterations;
};

MandelPoint* createMandelPoint(uint32_t numPoints)
{
    return malloc(numPoints * sizeof(MandelPoint));
}

MandelPoint* indexMandelPoint(MandelPoint* points, int num)
{
    return points + num;
}

// calculates one iteration for the given point
static inline void point_next(MandelPoint* p)
{
    if (p->diverged)
        return;

    p->z.im = 2.0 * p->z.re * p->z.im + p->c.im;
    p->z.re = p->z2.re - p->z2.im + p->c.re;
    p->z2.re = p->z.re * p->z.re;
    p->z2.im = p->z.im * p->z.im;

    if ((p->z2.re + p->z2.im > 4.0 ))
        p->diverged = p->iterations;
    else
        ++(p->iterations);
}

void iterateMandelbrot(MandelPoint* points, int numPoints, int iterations)
{
    while(numPoints--) {
        int i = iterations;
        while (i--) {
            point_next(points + numPoints);
        }
    }
}

void drawMandelbrot(const MandelPoint* points,
                    uint32_t*    pixels,
                    int          numPoints,
                    const uint32_t*    colors,
                    int          numColors)
{
    ptrdiff_t i = numPoints;
    while(i--) {
        pixels[i] = colors[points[i].diverged % numColors];
    }
}


void initMandelbrot(MandelPoint* points, const struct ScreenXY* screen)
{
    double mapX = (screen->xMax - screen->xMin) / (double)screen->width;
    double mapY = (screen->yMax - screen->yMin) / (double)screen->height;
    ptrdiff_t i = screen->height * screen->width;
    int h = screen->height;
    while (h--) {
        int w = screen->width;
        while (w--) {
            points[--i].c.re = (double)w * mapX + screen->xMin;
            points[i].c.im = (double)h * mapY + screen->yMin;
            memset(&points[i].z, 0, 2 * (sizeof(struct complexd) + sizeof(uint32_t)));
        }
    }
}
