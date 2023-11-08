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
	int ir_dist;
} point_t;

typedef struct object {
	int angle;
	int distance;
	double width;
	typedef struct object *next_object;
} object_t;

void scan(point_t points[]);

void find_objects(point_t points[]);

#endif /* SIMPLE_MISSION_H_ */
