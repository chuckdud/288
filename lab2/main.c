/*
 * main.c
 *
 *  Created on: Sep 8, 2023
 *      Author: cddudley
 */
#include "movement.h"

void main() {

    oi_t *sensor_data = oi_alloc();

    move_forward(sensor_data, 2000);

    oi_free(sensor_data);
}
