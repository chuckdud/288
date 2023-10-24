/*
 * movment.c
 *
 */
#include "movement.h"
#include "open_interface.h"
#include "Timer.h"
#include "lcd.h"

int bumped(oi_t *sensor) {
    if (sensor->bumpLeft || sensor->bumpRight) return 1;
    return 0;
}

void avoid_obstacle(oi_t *sensor) {
    int left_bump = sensor->bumpLeft;
    int backtrack = 150;
    move_backward(sensor, backtrack);
    if (left_bump) {
        // turn right 90, move away from obstacle, turn left 90
        turn_clockwise(sensor, 90);
        move_forward(sensor, 150);
        turn_counter_clockwise(sensor, 90);
    } else {
        // turn left 90, move away from obstacle, turn right 90
        turn_counter_clockwise(sensor, 90);
        move_forward(sensor, 250);
        turn_clockwise(sensor, 90);
    }
    move_forward(sensor, backtrack);
    oi_setWheels(0, 0);
}

void move_backward(oi_t *sensor, int milimeters){
    if (milimeters < 0) return;
    oi_init(sensor);
    double sum = 0;
    int backward_speed = -50;
    oi_setWheels(backward_speed, backward_speed);
    while ((-1 * sum) < milimeters) {
        oi_update(sensor);
        sum += sensor->distance;
    }
    oi_setWheels(0, 0); // stop
}

void move_forward(oi_t *sensor, int milimeters){
    if (milimeters < 0) return;
    oi_init(sensor);
    double sum = 0;
    int forward_speed = 150;
    oi_setWheels(forward_speed, forward_speed);
    while (sum < milimeters) {
        oi_update(sensor);
        if (bumped(sensor)) {
            oi_setWheels(0, 0);
            avoid_obstacle(sensor);
            oi_setWheels(forward_speed, forward_speed);
            oi_init(sensor);
        }
        sum += sensor->distance;
//        oi_init(sensor);
    }
    oi_setWheels(0, 0); // stop
}

void turn_clockwise(oi_t *sensor, int degrees){
    if (degrees < 0) return;
   oi_init(sensor);
   double turned = 0;
   oi_setWheels(-40, 40);

   while(degrees-4 > (-1 * turned)){
       oi_update(sensor);
       turned += sensor->angle;
   }
   oi_setWheels(0, 0);
}

void turn_counter_clockwise(oi_t *sensor, int degrees){
    if (degrees < 0) return;
   oi_init(sensor);
   double turned = 0;
   oi_setWheels(40, -40);

   while(turned < degrees-4){
       oi_update(sensor);
       turned += sensor->angle;
   }
   oi_setWheels(0, 0);
}
