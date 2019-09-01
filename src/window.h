/** @file        window.h
 *
 *  @brief       Create SDL2 fullscreen window and render to it.
 *
 *  @version     1.0
 *  @date        07/12/2019
 *  Revision:    -
 *
 *  @author      René Heldmaier
 *  @copyright   Copyright (c) 2019 René Heldmaier. All rights reserved.
 *               This work is licensed under the terms of the MIT license.
 */

#ifndef WINDOW_H

#include <stdint.h>

/**
 * Contains internal window data.
 * Must be created by a window_create function and freed by window_destroy.
 * Should only be modified indirectly by window functions.
 */

typedef struct window Window;

/** @brief Creates a window
 *
 *  Must be freed with window_destroy.
 *  The new Window defaults to fullscreen with desktop resolution.
 *
 *  @param name     Window name
 *  @return         Pointer to newly created window
 */

Window* window_create(const char* name);

/** @brief Close the window and free resources
*
*   @param window
*   @return
*/

void window_destroy(Window* window);

/** @brief Displays the pixels in buffer on the window
*
*   @param window
*   @param pixels - Must at least have width*height elements
*   @return 0 on success
*/

int window_update(Window* window, const uint32_t* pixels);

/** @brief Returns the height of the window
*
*   @param window
*   @return window height
*/

int window_getHeight(Window* window);

/** @brief Returns the width of the window
*
*   @param window
*   @return window width
*/

int window_getWidth(Window* window);

/** @brief Returns a string with a description of the last occured error.
*
*   @param void
*   @return Statically allocated error message string. Do not free.
*/

const char* window_getError();

#define WINDOW_H
#endif /* WINDOW_H */
