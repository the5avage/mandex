/*  Filename:  window.c
 *
 *  Author:    René Heldmaier
 *  Copyright: (c) 2019 René Heldmaier. All rights reserved.
 *             This work is licensed under the terms of the MIT license.
 */

#include "window.h"
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

struct window
{
    SDL_Window*     sdlWindow;
    SDL_Texture*    texture;
    SDL_Renderer*   renderer;
    int             width;
    int             height;
};

// Points to string containg the last error message
const char* errmsg;

// for each open window 1 is added
// if 0 window_create initialises SDL2
// if 1 window_destroy quits SDL2
int sdl_initialised = 0;

static int initSDL() {
    if (sdl_initialised++ == 0) {
        if (SDL_Init(SDL_INIT_VIDEO)) {
            errmsg = SDL_GetError();
            return 1;
        }
    }
    return 0;
}

// both window_create functions have to do this
static int window_init(Window* window)
{
    window->renderer = SDL_CreateRenderer(window->sdlWindow, -1,
                                            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (window->renderer == NULL) {
        errmsg = SDL_GetError();
        SDL_DestroyWindow(window->sdlWindow);
        free(window);
        return 1;
    }
    window->texture = SDL_CreateTexture(window->renderer,
                                        SDL_PIXELFORMAT_RGBA8888,
                                        SDL_TEXTUREACCESS_STREAMING,
                                        window->width,
                                        window->height);
    if (window->texture == NULL) {
        errmsg = SDL_GetError();
        SDL_DestroyWindow(window->sdlWindow);
        SDL_DestroyRenderer(window->renderer);
        free(window);
        return 1;
    }
    return 0;
}

Window* window_create(const char* name)
{
    if (initSDL())
        return NULL;

    Window* window = malloc(sizeof(Window));
    if (window == NULL) {
        errmsg = strerror(errno);
        return NULL;
    }

    window->sdlWindow = SDL_CreateWindow(name,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    0, 0,
                                    SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_HIDDEN);
    if (window->sdlWindow == NULL) {
        errmsg = SDL_GetError();
        free(window);
        return NULL;
    }
    SDL_GetWindowSize(window->sdlWindow, &(window->width), &(window->height));

    if (window_init(window))
        return NULL;

    return window;
}

int window_setMode(Window* window, enum Window_Mode mode)
{
    if (mode & Window_shown) {
        SDL_ShowWindow(window->sdlWindow);
    }
    if (mode & Window_hideCursor) {
        SDL_ShowCursor(SDL_DISABLE);
    }
    return 0;
}

void window_destroy(Window* window)
{
    SDL_DestroyRenderer(window->renderer);
    SDL_DestroyWindow(window->sdlWindow);
    SDL_DestroyTexture(window->texture);
    free(window);
    // calls SDL_Quit if neccessary
    if (--sdl_initialised == 0)
        SDL_Quit();
}

int window_update(Window* window, uint32_t* pixels)
{
    int check = SDL_UpdateTexture( window->texture,
                                    NULL,
                                    pixels,
                                    window->width * sizeof(uint32_t));
    if (check) {
        errmsg = SDL_GetError();
        return -1;
    }
    if (SDL_RenderClear(window->renderer)) {
        errmsg = SDL_GetError();
        return -1;
    }
    if (SDL_RenderCopy(window->renderer, window->texture, NULL, NULL)) {
        errmsg = SDL_GetError();
        return -1;
    }
    SDL_RenderPresent(window->renderer);
    return 0;
}

int window_getHeight(Window* window)
{
    return window->height;
}

int window_getWidth(Window* window)
{
    return window->width;
}

const char* window_getError()
{
    return errmsg;
}
