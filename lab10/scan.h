/*
 * simple_mission.h
 *
 *  Created on: Nov 7, 2023
 *      Author: cddudley
 */

#ifndef SIMPLE_MISSION_H_
#define SIMPLE_MISSION_H_

typedef struct point {
	int angle;
	float ping_dist;
	int ir_dist;
} point_t;

typedef struct object {
    int number;
	int angle;
	float distance;
	double width;
//	typedef struct object *next_object;
} object_t;

void scan(int angle, point_t *point);

void scan_180(point_t points[]);

void print_180(const point_t points[91]);

void find_objects(point_t points[]);

// old implementations - need to be improved
void find_smallest_object(point_t points[91],struct object objects[4]);

void scan_Ping(struct object objects[4]);

int get_object_count();

void driveToSmallest(oi_t *sensor,struct object objects[4]);

#endif /* SIMPLE_MISSION_H_ */
