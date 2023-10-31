
/*
 * ping.c ian's
 *
 *  Created on: Oct 25, 2023
 *      Author: ianlouis
 */
#include "Timer.h"
volatile int timeFallingEdge = 0;
volatile int timeRisingEdge = 0;
void ping_interrupt_handler();
int edgeCount = 0;

void ping_init(){
    timer_init();
    SYSCTL_RCGCGPIO_R |= 0x2; // enable clock port B
    GPIO_PORTB_AFSEL_R &= 0xFFFFFFF7; // pin 3 GPIO controlled
    GPIO_PORTB_DEN_R |=  0x08; // pin 3 enable digital func
    SYSCTL_RCGCTIMER_R |= 0x8; // give clock to timer 3

    // disable timer 3B
    TIMER3_CTL_R &= 0xFFFFFEFF;

    // use 16 bit timer
    TIMER3_CFG_R = TIMER_CFG_16_BIT; 
    // set TIMER3B input periodic, edge-time mode, count down
    TIMER3_TBMR_R =  (TIMER3_TBMR_R | 0x106) & 0xFFFFFF106;
    // set TIMER3B to get both edges
    TIMER3_CTL_R |= 0xC00;
    // load max val for count down
    TIMER3_TBILR_R = (TIMER3_TBILR_R | 0xFFF) & 0xFFFFFFFF; 
    // load max val for prescaler
    TIMER3_TBPR_R = (TIMER3_TBPR_R | 0xFF) & 0xFFFFFFFF; 
    // enable interrupts for timer B capture mode event interrupt only
    TIMER3_IMR_R |= 0x400;
    // bind interrupt to our handler
    NVIC_EN1_R |= 0b1000;
    IntRegister(INT_TIMER3B, ping_interrupt_handler);

    // enable timer 3B
    TIMER3_CTL_R |= 0x100;
}

void trigger_ping(){
    // set GPIO PB3 as output
    GPIO_PORTB_DIR_R |= 0x08;

    // send 20 microsecond pulse to activate sensor
    GPIO_PORTB_DATA_R |= 0x08;
    timer_waitMicros(20);
    GPIO_PORTB_DATA_R &= 0xFFFFFFF7;

    // set GPIO PB3 as input and prepare to receive signal
    GPIO_PORTB_DIR_R &= 0xFFFFFFF7;
    GPIO_PORTB_AFSEL_R |= 0x08;
    GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R | 0x7000) & 0xFFF7FFF; // set timer digital func
    //clear Timer B interrupts
    TIMER3_ICR_R |= 0xF00;
}

void ping_interrupt_handler() {
    if(TIMER3_MIS_R & 0x200 || TIMER3_MIS_R  &0x400){
        if(edgeCount == 0){
            timeRisingEdge = TIMER3_TBR_R;
            edgeCount++;
        }
        else if(edgeCount == 1){
            timeFallingEdge = TIMER3_TBR_R;
            edgeCount = 0;
        }
    }
    TIMER3_ICR_R |= 0xF00;
}
