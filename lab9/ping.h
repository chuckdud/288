/*
 * ping.h
 *
 *  Created on: Oct 31, 2023
 *      Author: cddudley
 */

#ifndef PING_H_
#define PING_H_

void ping_init(void);

void trigger_ping(void);

void ping_interrupt_handler(void);

#endif /* PING_H_ */
