/*
 * adc.c
 *
 *  Created on: Oct 18, 2023
 *      Author: cddudley
 */
#include "adc.h"
#include "math.h"

void adc_init() {
    SYSCTL_RCGCGPIO_R |= 0x2; // enable GPIOB clock
    SYSCTL_RCGCADC_R |= 0x1; // enable ADC0 clock

    GPIO_PORTB_AFSEL_R |= 0x10; // allows alternate function control

    GPIO_PORTB_DEN_R &= 0xEF; // disable digital function

    GPIO_PORTB_DIR_R &= 0xEF; // set pin 4 as input

    GPIO_PORTB_AMSEL_R |= 0x10; // use analog alternate functions

    GPIO_PORTB_ADCCTL_R |= 0x10; // turns on acd for port B

    ADC0_ACTSS_R &= 0xE;

    ADC0_EMUX_R &= 0xFFF0;

    ADC0_IM_R |= 0x1;

    //    ADC0_TSSEL_R - don't need because we are using default trigger

    ADC0_SSMUX0_R = (ADC0_SSMUX0_R & 0xFFFFFFF0) | 0xA;

    ADC0_SSCTL0_R = (ADC0_SSCTL0_R & 0xFFFFFFF6)|0x6; // sets the interrupt and end bits

    ADC0_SAC_R |= 0x04; //hardware average

    ADC0_ACTSS_R |= 0x1;
}

int adc_read(){
    ADC0_PSSI_R |= 0x1;
    while (ADC0_SSFSTAT0_R & 0x100);
    int adcValue = ADC0_SSFIFO0_R;
    ADC0_ISC_R |= 0x1;
    return adcValue &= 0x00000FFF;
}

int adc_read_convert() {
    int raw = adc_read();
    //    y = 11.29948 + (188820000 - 11.29948)/(1 + (x/15.11238)^3.667047)
    // 11.29948 + (188820000 - 11.29948) / (1 + pow(raw / 15.11238, 3.667047));
    return -7.79994*pow(10,-9)*pow(raw,3)+0.0000540086*pow(raw,2)-0.126712*raw+116.479;
}
