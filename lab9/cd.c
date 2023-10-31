/*
 * main.c
 *
 *  Created on: Sep 8, 2023
 *      Author: cddudley
 */
#include "movement.h"
#include "cyBot_Scan.h"
#include "Timer.h"
#include "lcd.h"
#include "stdlib.h"
#include "limits.h"
#include "cd.h"
#include "uart.h"
#include "open_interface.h"
char to_print[50];
//int pingAngle[5];
int object_count = 0;
int bg_min = 60; // min distance to be considered background


int raw_to_dist(int raw) {
    //    y = 11.29948 + (188820000 - 11.29948)/(1 + (x/15.11238)^3.667047)
    return  11.29948 + (188820000 - 11.29948) / (1 + pow(raw / 15.11238, 3.667047));
}

void print_180(const point_t points[91]) {
    //char to_print[50];

    int i;
    for (i = 0; i < 91; i++) {
        sprintf(to_print, "\r%d\t\t\t%d\n\r", points[i].angle, points[i].ir_dist);
        uart_sendStr(to_print);
    }
}

void scan_180(point_t points[]) {
    cyBOT_Scan_t data;
    int angle;
    int i = 0;
    int k = 0;

    for (angle = 0;  angle <= 180; angle += 2) {
        point_t p;
        p.angle = angle;
        p.ir_dist = 0;
        for (k = 0; k < 3; k++) {
            cyBOT_Scan(angle, &data);
            p.ir_dist += data.IR_raw_val;
        }
        p.ir_dist /= 3;
        p.ir_dist = raw_to_dist(data.IR_raw_val);
        points[i] = p;
        i++;
    }
}

void find_smallest_object(point_t points[91], struct object objects[4]) {
    object_count = 0;

    int i = 0;


    double actual_width = 0;
    int distance = 0;
    int angle_width = 0;

    point_t start_pt;
    point_t end_pt;

    while (i < 91) {
        if (points[i].ir_dist < bg_min) {
            actual_width = 0;
            distance = angle_width = 0;
            start_pt = points[i];
            end_pt = start_pt;
            while (i < 91 && abs(points[i].ir_dist - start_pt.ir_dist) < 10) {
                end_pt = points[i];
                i++;
            }
            if (i > 90) break;
            distance = (start_pt.ir_dist+end_pt.ir_dist) / 2;
            angle_width = end_pt.angle - start_pt.angle;
            actual_width = (double)angle_width / 2;
            actual_width = sin(actual_width * (3.1415 / 180));
            actual_width = actual_width * distance * (double)2;

            //if (angle_width > 5) {
            if (actual_width >= 2.5) {

                objects[object_count].number = object_count+1;
                objects[object_count].distance = distance;
                objects[object_count].actualWidth = actual_width;
                objects[object_count].direction = (start_pt.angle+end_pt.angle)/2;
               // pingAngle[object_count] =objects[object_count].direction;
                sprintf(to_print, "Object: %d\n\rDistance: %d\n\rActual Width:%.1f\n\rDirection: %d\n\n\r",  objects[object_count].number,objects[object_count].distance, objects[object_count].actualWidth,objects[object_count].direction);
                uart_sendStr(to_print);
                object_count++;
            }

        }
        i++;
    }
}
void scan_Ping(struct object objects[4]){
    cyBOT_Scan_t data;
     int z = 0;
//        cyBOT_Scan(0,&data);

        timer_waitMicros(1000000);
     while(z < object_count){
        cyBOT_Scan(objects[z].direction, &data);
        objects[z].pingDist = data.sound_dist;
        sprintf(to_print,"\n\rObject:%d Direction:%d  Distance:%.1f",z+1,objects[z].direction,objects[z].pingDist);
        uart_sendStr(to_print);
        z++;
        timer_waitMicros(1000000);
     }
    }

int get_object_count(){
    return object_count;
}


void driveToSmallest(oi_t *sensor, struct object objects[4]){
    sprintf(to_print,"num_objects: %d", get_object_count());
    uart_sendStr(to_print);
    if (get_object_count() == 0) {
        move_forward(sensor, bg_min * 10);
        return;
    }
    int smallestIndex = 0;
    int k = 1;
    while(k < get_object_count()){
        if(get_object_count() ==1){
         break;
        }
        else if(objects[k].actualWidth<objects[smallestIndex].actualWidth){
            smallestIndex = k;
        }
        k++;
    }
    if(objects[smallestIndex].direction > 90){
        uart_sendStr("\n\rCounter Clockwise");
        turn_counter_clockwise(sensor,objects[smallestIndex].direction-90);
        move_forward(sensor,(objects[smallestIndex].distance-12) *10);
    }
    else{
        uart_sendStr("\n\rClockwise");
        turn_clockwise(sensor,90-objects[smallestIndex].direction);
        move_forward(sensor,(objects[smallestIndex].distance-12)*10);

    }
 }
