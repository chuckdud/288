/*
 * cd.h
 *
 *  Created on: Oct 11, 2023
 *      Author: cddudley
 */

#ifndef CD_H_
#define CD_H_

typedef struct point {
    int angle;
    float sound_dist;
    int ir_dist;
} point_t;

//typedef struct object_list {
//    int num_objects;
//    point_t *objects;
//} object_list_t;

int raw_to_dist(int raw);

void print_180(const point_t points[91]);

//void print_objects(object_list_t object_list);

void scan_180(point_t points[91]);

void find_smallest_object(point_t points[91]);

#endif /* CD_H_ */
