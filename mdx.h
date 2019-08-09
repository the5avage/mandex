/** @file        mdx.h
 *
 *  @brief       The application specific functions for the mandelbrot explorer.
 *
 *  @version     1.0
 *  @date        07/14/2019
 *  Revision:    -
 *
 *  @author      René Heldmaier
 *  @copyright   Copyright (c) 2019 René Heldmaier. All rights reserved.
 *               This work is licensed under the terms of the MIT license.
 */

#ifndef MDX_H

#include <SDL2/SDL_stdinc.h>

/** @brief Possible styles for the color palette.
*
*   COLOR_RANDOM - colors are completly random. Mandelbrot set looks sharp.
*   COLOR_SMOOTH - colors are random but with a smooth gradient.
*/

enum color_style {
    MDX_COLOR_RANDOM,
    MDX_COLOR_SMOOTH
};

/** @brief Initialises the application and starts calculating the mandelbrotset
 *         with threads in the background.
 *
 *  @param screen_width Width of the screen to render
 *  @param screen_height Height of the screen to render
 *  @param The depth of the color palette (number of differnt colors)
 *  @param The style of the color palette (e.g. COLOR_RANDOM or COLOR_SMOOTH)
 *  @return void
 */

int mdx_run(int screen_width, int screen_height, int color_depth, int color_style);

/** @brief Stops the background threads and frees resources
 *
 *  @return void
 */

void mdx_quit(void);

/** @brief Gets events and acts accodingly
 *
 *  @param void
 *  @return true if user wants exit the application
 */

int mdx_event(void);

/** @brief Renders the screen to a pixel buffer
 *
 *  @param pixels Array which is filled
 *  @return
 */

void mdx_render(uint32_t* pixels);

/** @brief Moves the visible section upwards
*
*   @param void
*   @return void
*/

#define MDX_H
#endif /* MDX_H */
