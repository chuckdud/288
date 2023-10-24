/*
 * main.c
 *
 *  Created on: Sep 8, 2023
 *      Author: cddudley
 */
#include "movement.h"
#include "cyBot_uart.h"
#include "cyBot_Scan.h"
#include "Timer.h"
#include "lcd.h"
#include "stdlib.h"
#include "limits.h"

int raw_to_dist(int raw) {
    //    y = 11.29948 + (188820000 - 11.29948)/(1 + (x/15.11238)^3.667047)
    return  11.29948 + (188820000 - 11.29948) / (1 + pow(raw / 15.11238, 3.667047));
}

typedef struct point {
    int angle;
    float sound_dist;
    int ir_dist;
} point_t;

typedef struct object_list {
    int num_objects;
    point_t *objects;
} object_list_t;


void print_to_putty(char string[]) {
    int i = 0;
    while(i < strlen(string)){
        cyBot_sendByte(string[i]);
        i++;
    }
}

void print_pt(const point_t p) {
    char to_print[50];

    sprintf(to_print, "\r%d\t\t%.1f\t\t%d\n\r", p.angle, p.sound_dist, p.ir_dist);
    print_to_putty(to_print);
}


void print_180(const point_t points[91]) {
    int i;
    for (i = 0; i < 91; i++) {
        print_pt(points[i]);
    }
}

void print_objects(object_list_t object_list) {
    int i;
    for (i = 0; i < object_list.num_objects; i++) {
        print_pt(object_list.objects[i]);
    }
}


void scan_180(point_t points[91]) {
    cyBOT_Scan_t data;
    int angle;
    int i = 0;

    for (angle = 0;  angle <= 180; angle += 2) {
        cyBOT_Scan(angle, &data);
        point_t p;
        p.angle = angle;
        p.sound_dist = data.sound_dist;
        p.ir_dist = raw_to_dist(data.IR_raw_val);
        points[i] = p;
        i++;
    }
}


//object_list_t find_objects(point_t points[91]) {
//    int i;
//
//    float min_change_val = 7.0; // minimum change in distance to consider a new object
//    float bg_estimate = 55.0; // estimate of distance to background (non-object)
//
//    float change = 0.0;
//    int obj_count = 0;
//    float prev_dist = points[0].distance;
//    point_t objects[10];
//
//    for (i = 1; i < 91; i++) {
//        change = abs(points[i].distance - prev_dist);
//        if (points[i].distance < bg_estimate && change > min_change_val) {
//            // found new obj
//            objects[obj_count] = points[i];
//            obj_count++;
//
////            char to_print[15];
////            sprintf(to_print, "\rObject: %d\n", obj_count);
////            print_to_putty(to_print);
////            print_pt(points[i]);
//
//        }
//        prev_dist = points[i].distance;
//    }
//    object_list_t ret;
//    ret.objects = objects;
//    ret.num_objects = obj_count;
//    return ret;
//}

void find_smallest_object(point_t points[91]) {
    int bg_min = 80; // min distance to be considered background

    int i = 0;
    int object_count = 0;
    char end_flag = 0;
    while (i < 91) {
        if (points[i].ir_dist < bg_min) {
            point_t start_pt = points[i];
            point_t end_pt = start_pt;
            while (i < 91 && abs(points[i].ir_dist - start_pt.ir_dist) < 10) {
                end_pt = points[i];
                i++;
            }
            int rough_angle_width = end_pt.angle - start_pt.angle;
            if (rough_angle_width > 5) {
                object_count++;
                char to_print[15];
                sprintf(to_print, "Object: %d\n\rDistance: %d\n\rAngle Width: %d\n\rDirection: %d\n\n\r", object_count, (start_pt.ir_dist+end_pt.ir_dist)/2, rough_angle_width, (start_pt.angle+end_pt.angle)/2);
                print_to_putty(to_print);
            }
        }
        i++;
    }
}


/**
 * return angle of smallest object
 */
void find_objects(point_t points[91]) {
    int smallest_object_width = INT_MAX;
    int smallest_object_angle = 0;
    int i;

    float min_change_val = 5.0; // minimum change in distance to consider a new object
    float bg_estimate = 70.0; // estimate of distance to background (non-object)

    float change = 0.0;
    int obj_count = -1;
    float prev_dist = points[0].ir_dist;

    i = 1;
    while (i < 91) {
//        change = abs(points[i].distance - prev_dist);
        if (points[i].ir_dist < bg_estimate) {
            // (likely) found new obj
            int start_ang = points[i].angle;
            int curr_obj_ang_width = 0;
            float obj_min_dist = points[i].ir_dist;
            int outliers = 0;
            prev_dist = points[i].ir_dist;
            i++;
            while (i < 91 && outliers < 3) {
                change = abs(points[i].ir_dist - prev_dist);
                // if sensor gives garbage data back, ignore it and continue reading object - allow twice
                if (change > min_change_val) {
                    outliers++;
                    i++;
                    continue;
                } else {
                    curr_obj_ang_width = points[i].angle - start_ang;
//                    curr_obj_ang_width += 2;
                    if (points[i].ir_dist < obj_min_dist) obj_min_dist = points[i].ir_dist;
                    prev_dist = points[i].ir_dist;
                    i++;
                }
            }

            if (curr_obj_ang_width > 6) {
                obj_count++;
                if (curr_obj_ang_width < smallest_object_width) {
                    smallest_object_width = curr_obj_ang_width;
                    smallest_object_angle = (start_ang + start_ang + curr_obj_ang_width) / 2;
                }
                char to_print[15];
                sprintf(to_print, "\n\rObject: %d\n\rDistance: %.1f\n\rStarting Angle: %d\n\rAngle Width: %d\n\r", obj_count, obj_min_dist, start_ang, curr_obj_ang_width);
                print_to_putty(to_print);
            }

        }
        prev_dist = points[i].ir_dist;
        i++;
    }

    char to_print[15];
    sprintf(to_print, "\n\rSmallest obj @ %d\n\r", smallest_object_angle);
    print_to_putty(to_print);
    cyBOT_Scan_t data;
    cyBOT_Scan(smallest_object_angle, &data);
}

void main() {

    oi_t *sensor_data = oi_alloc();
    timer_init();
    lcd_init();
    int init = 7;
    cyBot_uart_init();
    cyBOT_init_Scan(init);

//    cyBOT_SERVRO_cal_t calibration = cyBOT_SERVO_cal();
//    right_calibration_value = calibration.right;
//    left_calibration_value = calibration.left;

    right_calibration_value = 248500;
    left_calibration_value = 1193500;

    int message = cyBot_getByte();

    if (message == 'm') {
        print_to_putty("\rDegrees\t\tDistance(cm)\n");
        point_t points[91];
        scan_180(points);
        print_180(points);
//        find_objects(points);
        find_smallest_object(points);
//        cyBOT_Scan_t data;
//        cyBOT_Scan(20, &data);
//        object_list_t object_list = find_objects(points);
//        print_objects(object_list);
    }
    oi_free(sensor_data);
}
