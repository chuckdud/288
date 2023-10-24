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

int raw_to_dist(int raw) {
    //    y = 11.29948 + (188820000 - 11.29948)/(1 + (x/15.11238)^3.667047)
    return  11.29948 + (188820000 - 11.29948) / (1 + pow(raw / 15.11238, 3.667047));
}

void print_180(const point_t points[91]) {
    //char to_print[50];

    int i;
    for (i = 0; i < 91; i++) {
        sprintf(to_print, "\r%d\t\t%.1f\t\t%d\n\r", points[i].angle, points[i].sound_dist, points[i].ir_dist);
        uart_sendStr(to_print);
    }
}

//void print_objects(object_list_t *object_list) {
//    int i;
//    for (i = 0; i < object_list.num_objects; i++) {
//        print_pt(object_list.objects[i]);
//    }
//}


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

void find_smallest_object(point_t points[91]) {
    int bg_min = 80; // min distance to be considered background

    int i = 0;
    int object_count = 0;
    char to_print[15];
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

            if (angle_width > 5) {
//            if (actual_width >= 5) {
                object_count++;
                sprintf(to_print, "Object: %d\n\rDistance: %d\n\rAngle Width:%.1f\n\rDirection: %d\n\n\r", object_count, distance, actual_width, (start_pt.angle+end_pt.angle)/2);
                uart_sendStr(to_print);
            }
        }
        i++;
    }
}

//
//void x() {
//
//    oi_t *sensor_data = oi_alloc();
//    timer_init();
//    lcd_init();
//    int init = 7;
//    uart_init(115200);
//    cyBOT_init_Scan(init);
//
//    cyBOT_SERVRO_cal_t calibration = cyBOT_SERVO_cal();
//    right_calibration_value = calibration.right;
//    left_calibration_value = calibration.left;
//
////    right_calibration_value = 248500;
////    left_calibration_value = 1193500;
//
//    char message = uart_receive();
//
//    if (message == 'm') {
//        uart_sendStr("\rDegrees\t\tDistance(cm)\n");
//        point_t points[91];
//        scan_180(points);
//        print_180(points);
////        find_objects(points);
//        find_smallest_object(points);
////        cyBOT_Scan_t data;
////        cyBOT_Scan(20, &data);
////        object_list_t object_list = find_objects(points);
////        print_objects(object_list);
//    }
//    oi_free(sensor_data);
//}
