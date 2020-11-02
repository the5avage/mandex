/*  Filename:  mandex.c
 *
 *  Author:    René Heldmaier
 *  Copyright: (c) 2019 René Heldmaier. All rights reserved.
 *             This work is licensed under the terms of the MIT license.
 */

#include <SDL2/SDL.h>
#include <emscripten.h>
#include "mdx.h"
#include "window.h"

#define FRAMERATE 30        //period in ms

void mainloop(void* arg)
{
    uint32_t* image = mdx_render();
    window_update(arg, image);
    mdx_event();
}

int main(int argc, char* argv[])
{
    Window* window = window_create("Fractal Explorer");
    if (window == NULL) {
        fprintf(stderr, "Create window failed %s\n", window_getError());
        return 1;
    }
    int width = window_getWidth(window);
    int height = window_getHeight(window);

    mdx_run(width, height, 10000, MDX_COLOR_SMOOTH);

    emscripten_set_main_loop_arg(mainloop, window, -1, 1);

    mdx_quit();
    window_destroy(window);
    return 0;
}
