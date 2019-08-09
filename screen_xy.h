/** @file        screen_xy.h
 *
 *  @brief       Mapping a screen with integer width and height to xy Coordinates
 *
 *  @version     1.0
 *  @date        07/28/2019
 *  Revision:    -
 *
 *  @author      René Heldmaier
 *  @copyright   Copyright (c) 2019 René Heldmaier. All rights reserved.
 *               This work is licensed under the terms of the MIT license.
 */

#ifndef SCREEN_XY_H

/** @brief Information about how pixels are mapped to xy coordinates
 */

struct ScreenXY {
    double xMin;
    double xMax;
    double yMin;
    double yMax;
    int width;
    int height;
};

/** @brief Moves up in xy-plane by a percentage of the displayed span
 *
 *  @param screen The screen to modify
 *  @param rate   The move rate in percent
 *  @return
 */

void moveUp(struct ScreenXY* screen, double rate);

/** @brief Moves down in xy-plane by a percentage of the displayed span
 *
 *  @param screen The screen to modify
 *  @param rate   The move rate in percent
 *  @return
 */

void moveDown(struct ScreenXY* screen, double rate);

/** @brief Moves left in xy-plane by a percentage of the displayed span
 *
 *  @param screen The screen to modify
 *  @param rate   The move rate in percent
 *  @return
 */

void moveLeft(struct ScreenXY* screen, double rate);

/** @brief Moves right in xy-plane by a percentage of the displayed span
 *
 *  @param screen The screen to modify
 *  @param rate   The move rate in percent
 *  @return
 */
void moveRight(struct ScreenXY* screen, double rate);

/** @brief Zooms in the xy-plane by a percantage of the displayed span
 *
 *  @param screen The screen to modify
 *  @param rate   The zoom rate in percent
 *  @return
 */

void zoomIn(struct ScreenXY* screen, double rate);

/** @brief Zooms in the xy-plane by a percantage of the displayed span
 *
 *  @param screen The screen to modify
 *  @param rate   The zoom rate in percent
 *  @return
 */

void zoomOut(struct ScreenXY* screen, double rate);

#define SCREEN_XY_H
#endif /* SCREEN_XY_H */
