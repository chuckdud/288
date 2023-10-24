/**
 * main.c
 */

#include "movement.h"
void square()
{
    oi_t *sensor_data = oi_alloc();

    move_forward(sensor_data, 500);
    turn_clockwise(sensor_data,90);
    move_forward(sensor_data, 500);
    turn_clockwise(sensor_data,90);
    move_forward(sensor_data, 500);
    turn_clockwise(sensor_data,90);
    move_forward(sensor_data, 500);
    turn_clockwise(sensor_data,90);


    oi_free(sensor_data);
}
