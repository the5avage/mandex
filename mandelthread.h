/** @file        mandelthread.h
 *
 *  @brief       Use multithreading to calculate mandelbrotset in the background.
 *
 *  @version     1.0
 *  @date        2019-08-30
 *  Revision:    -
 *
 *  @author      René Heldmaier
 *  @copyright   Copyright (c) 2019 René Heldmaier. All rights reserved.
 *               This work is licensed under the terms of the MIT license.
 */

#ifndef MANDELTHREAD_H
#define MANDELTHREAD_H

#include "screen_xy.h"

/** @brief  Starts calculating the mandelbrotset with threads in the background
 *
 *  @param  screen Information about wich pixel represents wich point in xy coordinates
 *  @return 0 if success
 */

int startThreads(struct ScreenXY* screen);

/** @brief  Changes the points for wich the mandelbrotset is calculated
 *
 *  @param  screen Information about wich pixel represents wich point in xy coordinates
 */

void changeMandel(struct ScreenXY* screen);

/** @brief  Draws the calculated mandelbrotset to the buffer according to color palette
 *
 *  @param  buffer_out The drawn mandelbrotset goes here. Size must be width * height
 *  @param  colors     The color palette
 *  @param  num_colors The size of the color palette
 */

void mandelthread_draw(uint32_t* buffer_out, uint32_t* colors, int num_colors);

/** @brief  Stops all threads and frees all resources
 */

void mandelthread_quit(void);

#endif /* MANDELTHREAD_H */