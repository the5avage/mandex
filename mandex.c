/*  Filename:  mandex.c
 *
 *  Author:    René Heldmaier
 *  Copyright: (c) 2019 René Heldmaier. All rights reserved.
 *             This work is licensed under the terms of the MIT license.
 */

#include <SDL2/SDL.h>
#include "mdx.h"
#include "window.h"

#define FRAMERATE 30        //period in ms

int main()
{
    Window* window = window_create("Fractal Explorer");
    if (window == NULL) {
        fprintf(stderr, "Create window failed %s\n", window_getError());
        return 1;
    }
    int width = window_getWidth(window);
    int height = window_getHeight(window);

    uint32_t* pixels = malloc(width * height * sizeof(uint32_t));
    if (pixels == NULL) {
        fprintf(stderr, "Fatal error: malloc failed\n");
        return 1;
    }

    mdx_run(width, height, 1000, MDX_COLOR_SMOOTH);
    for (;;) {
        uint32_t frame_start = SDL_GetTicks();
        mdx_render(pixels);
        window_update(window, pixels);
        if (mdx_event())
            break;
        uint32_t frame_time = SDL_GetTicks() - frame_start;
        if (frame_time < FRAMERATE)
            SDL_Delay(FRAMERATE - frame_time);
    }

    free(pixels);
    mdx_quit();
    window_destroy(window);
    return 0;
}
