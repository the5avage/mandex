/*  Filename:  mandelthread.c
 *
 *  Author:    René Heldmaier
 *  Copyright: (c) 2019 René Heldmaier. All rights reserved.
 *             This work is licensed under the terms of the MIT license.
 */

#include <stdlib.h>
#include <SDL2/SDL.h>
#include "mandelbrot.h"
#include "screen_xy.h"

// Two buffers of MandelPoint so one can get initialised to new
// values while threads still run on the other
MandelPoint* mandel_front;
MandelPoint* mandel_back;
int numMandelPoints;

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

// entry point for thread creation
static int threadFunction(void* data)
{
    struct ThreadData* trdata = data;
    while (trdata->run) {
        iterateMandelbrot(trdata->points, trdata->numPoints, 100);
    }
    return 0;
}

static void initThreadData()
{
    MandelPoint* mp = mandel_front;
    int allPoints = numMandelPoints;
    int thrdPoints = numMandelPoints / numThreads;
    for (int i = 0; i < numThreads - 1; ++i) {
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

void mandelthread_quit(void)
{
    for (int i = 0; i < numThreads; ++i) {
        stopThread(i);
    }
    free(threads);
    free(workData);
    free(mandel_back);
    free(mandel_front);
}

static int allocGlobals(void)
{
    mandel_front = createMandelPoint(numMandelPoints);
    if (!mandel_front) {
        return 1;
    }

    mandel_back = createMandelPoint(numMandelPoints);
    if (!mandel_back) {
        free(mandel_front);
        return 1;
    }

    threads = malloc(numThreads * sizeof(SDL_Thread*));
    if (!threads) {
        free(mandel_front);
        free(mandel_back);
        return 1;
    }

    workData = malloc(numThreads * sizeof(struct ThreadData));
    if (!workData) {
        free(mandel_front);
        free(mandel_back);
        free(threads);
        return 1;
    }
    return 0;
}

static int startThreads()
{
    for (int i = 0; i < numThreads; ++i) {
        threads[i] = SDL_CreateThread(threadFunction, "calculate Mandelbrot", &workData[i]);
        if (!threads[i]) {
            for (int j = 0; j < i; ++j) {  // close already spawned threads
                stopThread(j);
            }
            free(threads);
            free(workData);
            return 1;
        }
    }
    return 0;
}

int mandelthread_run(const struct ScreenXY* screen)
{
    numMandelPoints = screen->height * screen->width;
    numThreads = SDL_GetCPUCount();

    if (allocGlobals())
        return 1;

    initMandelbrot(mandel_front, screen);
    initThreadData();

    if(startThreads())
        return 2;

    return 0;
}

static inline void swap(MandelPoint** a, MandelPoint** b)
{
    MandelPoint* tmp = *a;
    *a = *b;
    *b = tmp;
}

static void changeThreadData(void)
{
    MandelPoint* tmp = mandel_front;
    int thrdPoints = numMandelPoints / numThreads;
    for (int i = 0; i < numThreads; ++i) {
        SDL_AtomicSetPtr((void**)&workData[i].points, tmp);
        tmp = indexMandelPoint(tmp, thrdPoints);
    }
}

void changeMandel(const struct ScreenXY* screen)
{
    swap(&mandel_front, &mandel_back);

    initMandelbrot(mandel_front, screen);

    changeThreadData();
}

void mandelthread_draw(uint32_t* buffer_out, const uint32_t* colors, int num_colors)
{
    drawMandelbrot(mandel_front, buffer_out, numMandelPoints, colors, num_colors);
}