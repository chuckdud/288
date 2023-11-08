/*
 * servo.c
 *
 *  Created on: Nov 1, 2023
 *      Author: cddudley
 */

#include "servo.h"
#include "math.h"
#include "Timer.h"

volatile int falling_edge = 0;

void servo_init() {
    timer_init();

    // CLOCKS
    SYSCTL_RCGCGPIO_R |= 0x2; // enable clock port B
    SYSCTL_RCGCTIMER_R |= 0x2; // give clock to timer 1

    // GPIO
    // pin 5 enable digital func
    GPIO_PORTB_DEN_R |=  0x20;
    // set alternate function to timer 1
    GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R & 0xFF0FFFFF) | 0x700000;
    // pin 5 GPIO controlled
    GPIO_PORTB_AFSEL_R |= 0x20;

    // TIMER
    // disable timer 1B
    TIMER1_CTL_R &= 0xFFFFFEFF;
    // use 16 bit timer
    TIMER1_CFG_R = TIMER_CFG_16_BIT;
    // set TIMER1B periodic, count down
    TIMER1_TBMR_R =  (TIMER1_TBMR_R | 0xD0B) & 0xFFFFFFD0B;
    // set TIMER1B to event mode reserved
    TIMER1_CTL_R |= 0xC00;

    // set period of PWM
    // load max val for count down
    TIMER1_TBILR_R = 0xE200;
    // load max val for prescaler
    TIMER1_TBPR_R = 0x4;
}

void servo_move(float degrees) {
    float pulse_width = (150.62 * degrees) + 1; // in clock cycles

    int pulse_width_int = round(pulse_width) + (0x4E200-311556);

    falling_edge = 0x4E200 - pulse_width_int;
    TIMER1_TBMATCHR_R = falling_edge;
    TIMER1_TBPMR_R = falling_edge >> 16;

    // enable timer 1B
    TIMER1_CTL_R |= 0x100;
    // allow time for servo to move to correct position
    timer_waitMillis(500);
}


