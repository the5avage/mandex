/*  Filename:  mdx.c
 *
 *  Author:    René Heldmaier
 *  Copyright: (c) 2019 René Heldmaier. All rights reserved.
 *             This work is licensed under the terms of the MIT license.
 */

#include <time.h>
#include <SDL2/SDL.h>
#include "mandelbrot.h"
#include "mdx.h"
#include "screen_xy.h"
#include "color_palette.h"
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

// Two buffers of MandelPoint so one can get initialised to new
// values while threads still run on the other
MandelPoint* mandel_front;
MandelPoint* mandel_back;
int numMandel;

// Contains the image
uint32_t* image_buffer;

// filename for .bmp file is saved here
char image_name[51];

// For each cpu core one thread is spawned which calculates the mandelbrot set
SDL_Thread** threads;
int numThreads;
// Threads work on this data
struct ThreadData {
    MandelPoint* points;
    int numPoints;
    int run;        //thread stops if this is zero
};
struct ThreadData* workData;

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

// entry point for thread creation
int threadFunction(void* data)
{
    struct ThreadData* trdata = data;
    for (;trdata->run;) {
        iterateMandelbrot(trdata->points, trdata->numPoints, 100);
    }
    return 0;
}

static void initThreadData(void)
{
    MandelPoint* mp = mandel_front;
    int allPoints = numMandel;
    int thrdPoints = numMandel / numThreads;
    for (int i = 0; i != numThreads - 1; ++i) {
        workData[i].numPoints = thrdPoints;
        allPoints -= thrdPoints;
        workData[i].points = mp;
        mp = indexMandelPoint(mp, thrdPoints);
        workData[i].run = 1;
    }
    workData[numThreads - 1].numPoints = allPoints; // in case numPoints is not divisible by numThreads
    workData[numThreads - 1].points = mp;
    workData[numThreads - 1].run = 1;
}

static void stopThread(int index)
{
    workData[index].run = 0;
    SDL_WaitThread(threads[index], NULL);
}

static int startThreads(void)
{
    numThreads = SDL_GetCPUCount();
    threads = malloc(numThreads * sizeof(SDL_Thread*));
    if (!threads) {
        errstr = erralloc;
        return 1;
    }
    workData = malloc(numThreads * sizeof(struct ThreadData));
    if (!workData) {
        free(threads);
        errstr = erralloc;
        return 1;
    }

    initThreadData();

    for (int i = 0; i != numThreads; ++i) {
        threads[i] = SDL_CreateThread(threadFunction, "calculate Mandelbrot", &workData[i]);
        if (!threads[i]) {
            errstr = errthrd;
            for (int j = 0; j != i; ++j) {  // close already spawned threads
               stopThread(j);
            }
            free(threads);
            free(workData);
            return 2;
        }
    }
    return 0;
}

int mdx_run(int screen_width, int screen_height, int color_depth, int color_style)
{
    screen.width = screen_width;
    screen.height = screen_height;
    numMandel = screen_height * screen_width;

    uint32_t* colors = malloc(sizeof(uint32_t) * color_depth);
    if (!colors) {
        errstr = erralloc;
        return 1;
    }
    initColorPalette(colors, color_depth, color_style);

    mandel_front =  createMandelPoint(screen_height * screen_width);
    if (!mandel_front) {
        free(colors);
        errstr = erralloc;
        return 1;
    }
    mandel_back = createMandelPoint(screen_height * screen_width);
    if (!mandel_back) {
        free(colors);
        free(mandel_front);
        errstr = erralloc;
        return 1;
    }
    initMandelbrot(mandel_front, &screen);

    image_buffer = malloc(screen_width * screen_height * sizeof(uint32_t));
    if (!image_buffer) {
        free(colors);
        free(mandel_front);
        free(mandel_back);
        errstr = erralloc;
    }

    if (startThreads()) {
        free(colors);
        free(mandel_front);
        free(mandel_back);
        free(image_buffer);
        return 1;
    }

    return 0;
}


void mdx_quit(void)
{
    for (int i = 0; i != numThreads; ++i) {
        stopThread(i);
    }
    free(threads);
    free(workData);
    free(colorPalette);
    free(mandel_back);
    free(mandel_front);
}

static void changeMandel(void)
{
    MandelPoint* tmp = mandel_back;
    mandel_back = mandel_front;
    mandel_front = tmp;

    initMandelbrot(mandel_front, &screen);

    int thrdPoints = numMandel / numThreads;
    for (int i = 0; i != numThreads; ++i) {
        SDL_AtomicSetPtr((void**)&workData[i].points, tmp);
        tmp = indexMandelPoint(tmp, thrdPoints);
    }
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
                changeMandel();
                break;
            case SDLK_UP:
                moveUp(&screen, move_rate);
                changeMandel();
                break;
            case SDLK_RIGHT:
                moveRight(&screen, move_rate);
                changeMandel();
                break;
            case SDLK_LEFT:
                moveLeft(&screen, move_rate);
                changeMandel();
                break;
            case SDLK_i:
                zoomIn(&screen, zoom_rate);
                changeMandel();
                break;
            case SDLK_o:
                zoomOut(&screen, zoom_rate);
                changeMandel();
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
    drawMandelbrot(mandel_front, image_buffer, numMandel, colorPalette, numColors);
    return image_buffer;
}
