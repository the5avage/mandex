/** @file        color_palette.h
 *
 *  @brief       Functions to fill arrays of colors with different styles
 *               (e.g. completely random, random with a smooth gradient)
 *
 *  @version     1.0
 *  @date        07/30/2019
 *  Revision:    -
 *
 *  @author      René Heldmaier
 *  @copyright   Copyright (c) 2019 René Heldmaier. All rights reserved.
 *               This work is licensed under the terms of the MIT license.
 */

#ifndef COLOR_PALETTE_H

/** @brief Creates a completely random color palette
*
*   @param Array which is filled with random colors. Must be allocated before.
*   @param The number of colors which are filled in the array.
*   @return void
*/

void colorRandom(uint32_t* colors, int num_colors);

/** @brief Creates a color palette with smooth gradients.
*
*   @param Array which is filled with random colors. Must be allocated before.
*   @param The number of colors which are filled in the array.
*   @return void
*/

void colorSmooth(uint32_t* colors, int num_colors);

#define COLOR_PALETTE_H
#endif /* COLOR_PALETTE_H */
