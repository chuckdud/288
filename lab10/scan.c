/*
 * simple_mission.c
 *
 *  Created on: Nov 7, 2023
 *      Author: cddudley
 */
#include "movement.h"
#include "uart.h"
#include "ping.h"
#include "scan.h"
#include "servo.h"
#include "adc.h"
#include "lcd.h"
#include "open_interface.h"
#include "stdlib.h"
#include "Timer.h"

char to_print[75];
int object_count = 0;
int bg_min = 60; // min distance to be considered background

float scan_ping(int angle) {
    servo_move(angle);
    return ping_read();
}

void scan_pt(int angle, point_t *point) {
    servo_move(angle);
    (*point).angle = angle;
    (*point).ir_dist = adc_read_convert();
    (*point).ping_dist = ping_read(); // not currently averaging
}

void scan_180(point_t points[91]) {
    int angle, i;
    i = 0;
    for (angle = 0; angle <= 180; angle+=2) {
        point_t p;
        scan_pt(angle, &p);
        points[i] = p;
        i++;
    }
}

void print_180(const point_t points[91]) {
    int i;
    for (i = 0; i < 91; i++) {
        sprintf(to_print, "\r%d\t\t\t%d\t\t\t%.1f\n", points[i].angle, points[i].ir_dist,points[i].ping_dist);
        uart_sendStr(to_print);
    }
}

//void find_objects(point_t points[], object_t *obj_list) {
//    int i = 0;
//
//    double actual_width = 0;
//    int distance = 0;
//    int angle_width = 0;
//
//    point_t start_pt;
//    point_t end_pt;
//
//    object_t obj;
//    *obj_list.next_object = &obj;
//
//    while (i < 91) {
//        if (points[i].ir_dist < bg_min) {
//            actual_width = distance = angle_width = 0;
//            start_pt = points[i];
//            end_pt = start_pt;
//            while (i < 91 && abs(points[i].ir_dist - start_pt.ir_dist) < 10) {
//                end_pt = points[i];
//                i++;
//            }
//            if (i > 90) break;
//            distance = (start_pt.ir_dist+end_pt.ir_dist) / 2;
//            angle_width = end_pt.angle - start_pt.angle;
//            actual_width = (double)angle_width / 2;
//            actual_width = sin(actual_width * (3.1415 / 180));
//            actual_width = actual_width * distance * (double)2;
//
//            //if (angle_width > 5) {
//            if (actual_width >= 2.5) {
//                obj.distance = distance;
//                obj.width = actual_width;
//                obj.angle = (start_pt.angle+end_pt.angle)/2;
//                sprintf(to_print, "Object: %d\n\rDistance: %d\n\rActual Width:%.1f\n\rDirection: %d\n\n\r",  objects[object_count].number,objects[object_count].distance, objects[object_count].width,objects[object_count].angle);
//                uart_sendStr(to_print);
//                object_count++;
//                object_t next;
//                obj.next_object = &next;
//                obj = *obj.next_object;
//            }
//
//        }
//        i++;
//    }
//}

void find_smallest_object(point_t points[91], struct object objects[4]) {
    object_count = 0;

    int i = 0;
    double actual_width;
    float distance;
    int angle;
    int angle_width;

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
            angle = (start_pt.angle+end_pt.angle)/2;
            distance = scan_ping(angle);
            angle_width = end_pt.angle - start_pt.angle;
            actual_width = (double)angle_width / 2;
            actual_width = sin(actual_width * (3.1415 / 180));
            actual_width = actual_width * distance * (double)2;

            //if (angle_width > 5) {
            if (actual_width >= 2.5) {
               if(distance < bg_min){
                objects[object_count].number = object_count+1;
                objects[object_count].distance = distance;
                objects[object_count].width = actual_width;
                objects[object_count].angle = angle;
                // pingAngle[object_count] =objects[object_count].angle;
                sprintf(to_print, "Object: %d\n\rDistance: %d\n\rWidth:%.1f\n\rDirection: %d\n\r",  objects[object_count].number,objects[object_count].distance, objects[object_count].width,objects[object_count].angle);
                uart_sendStr(to_print);
                object_count++;
               }
            }
        }
        if (object_count == 4) break;
        i++;
    }
}

void scan_Ping(struct object objects[4]){
    point_t point;
    int i = 0;
    while(i < object_count){
        scan_pt(objects[i].angle, &point);
        objects[i].distance = point.ping_dist;
        sprintf(to_print,"\n\rObject:%d Direction:%.1f  Distance:%d",i+1,objects[i].angle, objects[i].distance);
        uart_sendStr(to_print);
        i++;
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
        else if(objects[k].width<objects[smallestIndex].width){
            smallestIndex = k;
        }
        k++;
    }
    if(objects[smallestIndex].angle > 90){
        uart_sendStr("\n\rCounter Clockwise");
        turn_counter_clockwise(sensor,objects[smallestIndex].angle-90);
        move_forward(sensor,(objects[smallestIndex].distance-12) *10);
    }
    else{
        uart_sendStr("\n\rClockwise");
        turn_clockwise(sensor,90-objects[smallestIndex].angle);
        move_forward(sensor,(objects[smallestIndex].distance-12)*10);

    }
}
