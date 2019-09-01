/** @file        mandelbrot.h
 *
 *  @brief       Functions for calculating the mandelbrot set and display it in nice colors.
 *
 *  @version     1.0
 *  @date        07/14/2019
 *  Revision:    -
 *
 *  @author      René Heldmaier
 *  @copyright   Copyright (c) 2019 René Heldmaier. All rights reserved.
 *               This work is licensed under the terms of the MIT license.
 */

#ifndef MANDELBROT_H

#include <stdint.h>
#include "screen_xy.h"


/** @brief   Contains information for a complex point in mandelbrotset.
 *
 *  @details Each point corresponds to one pixel of the screen.
 */

typedef struct MandelPoint MandelPoint;

/** @brief Allocates memory for number of MandelPoints
 *
 *  @param number Number of elements
 *  @return Pointer to allocated memory. Must be freed manually.
 */

MandelPoint* createMandelPoint(uint32_t numPoints);

/** @brief Increments the pointer by a number of elements.
 *
 *         This is necesarry because the size of MandelPoint is not visible.
 *
 *  @param points Pointer to array.
 *  @param num    Array index
 *  @return       Pointer to element with index relative to poins
 */

MandelPoint* indexMandelPoint(MandelPoint* points, int num);

/** @brief Initialises the Mandelbrot Points.
 *         Must be called before use of iterateMandelbrot()
 *
 *  @param points The array of points. Must be allocated with to width * height elements
 *  @param screen Contains information about how screen is mapped to xy-coordinates
 *  @return void
 */

void initMandelbrot(MandelPoint* points, const struct ScreenXY* screen);

/** @brief Calculates mandelbrot iterations over given points.
*
*   @param  points     Array of points.
*   @param  numPoints  Number of elements in array points
*   @param  iterations Number of iterations which are calculated.
*   @return void
*/

void iterateMandelbrot(MandelPoint* points, int numPoints, int iterations);

/** @brief Draws the mandelbrot to an array of pixels
 *
 *  @param  points    The Mandelbrot points
 *  @param  pixels    The pixels which are drawn
 *  @param  numPoints Number of pixels and points must be the same
 *  @param  colors    The color palette
 *  @param  numColors Depth of the color palette
 *  @return void
 */

void drawMandelbrot(const MandelPoint* points,
                    uint32_t* pixels,
                    int numPoints,
                    const uint32_t* colors,
                    int numColors);

#define MANDELBROT_H
#endif /* MANDELBROT_H */
