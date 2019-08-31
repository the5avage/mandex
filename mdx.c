/*  Filename:  mdx.c
 *
 *  Author:    René Heldmaier
 *  Copyright: (c) 2019 René Heldmaier. All rights reserved.
 *             This work is licensed under the terms of the MIT license.
 */

#include <time.h>
#include <SDL2/SDL.h>
#include "mdx.h"
#include "screen_xy.h"
#include "color_palette.h"
#include "mandelthread.h"
#include "saveBmp.h"

// Part of xy-plane which is displayed on the screen
struct ScreenXY screen = {
    .xMin = -2.5,
    .xMax = 1.0,
    .yMin = -1.0,
    .yMax = 1.0
};

// Rate (percent) screen is modified at event
const double move_rate = 0.1;
const double zoom_rate = 0.05;

// Mandelbrot set is colored according to this palette
uint32_t* colorPalette;
int numColors;

// Contains the image
uint32_t* image_buffer;

// filename for .bmp file is saved here
char image_name[51];

// Contains error message
const char* errstr = "mdx: No error occured\n";
// Possible error messages
const char* erralloc = "mdx: Memory allocation failed\n";
const char* errthrd = "mdx: Thread creation failed\n";

static void initColorPalette(uint32_t* colors, int depth, int style)
{
    colorPalette = colors;
    numColors = depth;
    switch (style) {
    case MDX_COLOR_RANDOM:
        colorRandom(colors, depth);
        break;
    case MDX_COLOR_SMOOTH:
        colorSmooth(colors, depth);
        break;
    }
}

int mdx_run(int screen_width, int screen_height, int color_depth, int color_style)
{
    screen.width = screen_width;
    screen.height = screen_height;

    uint32_t* colors = malloc(sizeof(uint32_t) * color_depth);
    if (!colors) {
        errstr = erralloc;
        return 1;
    }
    initColorPalette(colors, color_depth, color_style);

    image_buffer = malloc(screen_width * screen_height * sizeof(uint32_t));
    if (!image_buffer) {
        free(colors);
        errstr = erralloc;
    }

    if (mandelthread_run(&screen)) {
        free(colors);
        free(image_buffer);
        errstr = errthrd;
        return 1;
    }

    return 0;
}

void mdx_quit(void)
{
    mandelthread_quit();
    free(colorPalette);
    free(image_buffer);
}

static void printMandel(void)
{
    sprintf(image_name, "%li.bmp", time(NULL));
    saveBMP(image_name, image_buffer, screen.width, -screen.height);
}

int mdx_event(void)
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            return 1;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                return 1;
            case SDLK_DOWN:
                moveDown(&screen, move_rate);
                changeMandel(&screen);
                break;
            case SDLK_UP:
                moveUp(&screen, move_rate);
                changeMandel(&screen);
                break;
            case SDLK_RIGHT:
                moveRight(&screen, move_rate);
                changeMandel(&screen);
                break;
            case SDLK_LEFT:
                moveLeft(&screen, move_rate);
                changeMandel(&screen);
                break;
            case SDLK_i:
                zoomIn(&screen, zoom_rate);
                changeMandel(&screen);
                break;
            case SDLK_o:
                zoomOut(&screen, zoom_rate);
                changeMandel(&screen);
                break;
            case SDLK_p:
                printMandel();
                break;
            }
            break;
        }
   }
   return 0;
}

uint32_t* mdx_render(void)
{
    mandelthread_draw(image_buffer, colorPalette, numColors);
    return image_buffer;
}
