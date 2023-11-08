/*
 * adc.h
 *
 *  Created on: Oct 18, 2023
 *      Author: cddudley
 */

#ifndef ADC_H_
#define ADC_H_

#include "Timer.h"

void adc_init(void);

int adc_read(void);

int adc_read_convert();

int raw_to_dist(int raw);

#endif /* ADC_H_ */
