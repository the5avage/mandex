/** @file        mdx.h
 *
 *  @brief       The application specific functions for the mandelbrot explorer.
 *
 *  @version     1.0
 *  @date        2019/08/10
 *  Revision:    -
 *
 *  @author      René Heldmaier
 *  @copyright   Copyright (c) 2019René Heldmaier. All rights reserved.
 *               This work is licensed under the terms of the MIT license.
 */

#ifndef SAVEBMP_H
#define SAVEBMP_H

#include <stdint.h>

/** @brief Converts an image from an rgba buffer to bmp file format and saves it.
 *
 *  If height is negative the y-axis starts at the top of the screen and increases downwards.
 *
 *  @param filename   The name or path of the file.
 *  @param pixels     Array of pixels containing the image.
 *  @param width      The image width in pixels
 *  @param height     The image height in pixels. Can be negative to invert y-axis
 *
 *  @return 0 on sucess
 */

int saveBMP(const char* filename, const uint32_t* pixels, int32_t width, int32_t height);

/** @brief Converts an image from an rgba buffer to bmp file format and loads it
 *         into a malloced buffer
 *
 *  This function is called by saveBMP internally.
 *  Notice that the bmp data starts at an offset of two bytes
 *  because of alingment reasons.
 *
 *  @param pixels   Array of input pixels in rgba.
 *                  Must have width*height elements.
 *  @param width    Image width in pixels.
 *  @param height   Image height in pixels. Can be negative to invert y-axis.
 *  @param bmpSize  The size of the created bmp is written here.
 *
 *  @return Pointer to the newly created buffer.
 *          The bmp data starts at an offset of 2 bytes
 */

uint8_t* createBMP(const uint32_t* pixels, int32_t width, int32_t height, uint32_t* bmpSize);

#endif /* SAVEBMP_H */
