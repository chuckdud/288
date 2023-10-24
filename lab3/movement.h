/*
 * movement.h
 *
 *  Created on: Sep 8, 2023
 *      Author: cddudley
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_

#include "open_interface.h"

int bumped(oi_t *sensor);

void avoid_obstacle(oi_t *sensor);

void move_forward(oi_t *sensor, int milimeters);
void move_backward(oi_t *sensor, int milimeters);


void turn_clockwise(oi_t *sensor, int degrees);
void turn_counter_clockwise(oi_t *sensor, int degrees);

#endif /* MOVEMENT_H_ */
