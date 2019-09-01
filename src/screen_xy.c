/*  Filename:  screen_xy.c
 *
 *  Author:    René Heldmaier
 *  Copyright: (c) 2019 René Heldmaier. All rights reserved.
 *             This work is licensed under the terms of the MIT license.
 */

#include "screen_xy.h"


void moveUp(struct ScreenXY* screen, double rate)
{
    double temp = (screen->yMax - screen->yMin) * rate;
    screen->yMax -= temp;
    screen->yMin -= temp;
}

void moveDown(struct ScreenXY* screen, double rate)
{
    double temp = (screen->yMax - screen->yMin) * rate;
    screen->yMax += temp;
    screen->yMin += temp;
}

void moveLeft(struct ScreenXY* screen, double rate)
{
    double temp = (screen->xMax - screen->xMin) * rate;
    screen->xMax -= temp;
    screen->xMin -= temp;
}

void moveRight(struct ScreenXY* screen, double rate)
{
    double temp = (screen->xMax - screen->xMin) * rate;
    screen->xMax += temp;
    screen->xMin += temp;
}

void zoomIn(struct ScreenXY* screen, double rate)
{
    double temp = (screen->xMax - screen->xMin) * rate;
    screen->xMax -= temp;
    screen->xMin += temp;
    temp = (screen->yMax - screen->yMin) * rate;
    screen->yMax -= temp;
    screen->yMin += temp;
}

void zoomOut(struct ScreenXY* screen, double rate)
{
    double temp = (screen->xMax - screen->xMin) * rate;
    screen->xMax += temp;
    screen->xMin -= temp;
    temp = (screen->yMax - screen->yMin) * rate;
    screen->yMax += temp;
    screen->yMin -= temp;
}
