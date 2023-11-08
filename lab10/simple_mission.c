/*
 * simple_mission.c
 *
 *  Created on: Nov 7, 2023
 *      Author: cddudley
 */

#include "simple_mission.h"
#include "ping.h"
#include "servo.h"

void scan(point_t points[]) {
	int angle, i;
	i = 0;
	for (angle = 0; angle <= 180; angle+=2) {
		servo_move(angle);
		point_t p;
		p.angle = angle;
		p.ir_dist = ping_read(); // not currently averaging
		points[i] = p;
		i++;
	}
}

void find_objects(point_t points[]) {
	int i;
	for (i = 0; i < 91; i++) {
		
	}
}
