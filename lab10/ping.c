
/*
 * ping.c ian's
 *
 *  Created on: Oct 25, 2023
 *      Author: ianlouis
 */
#include "Timer.h"
#include "lcd.h"
#include "ping.h"
#include "driverlib/interrupt.h"

volatile int timeFallingEdge = 0;
volatile int timeRisingEdge = 0;

volatile int edgeCount = 0;

void ping_init(){
    timer_init();

    // CLOCKS
    SYSCTL_RCGCGPIO_R |= 0x2; // enable clock port B
    SYSCTL_RCGCTIMER_R |= 0x8; // give clock to timer 3

    // GPIO
    GPIO_PORTB_DEN_R |=  0x08; // pin 3 enable digital func
    GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R & 0xFFFF0FFF) | 0x7000;
    GPIO_PORTB_AFSEL_R &= 0xFFFFFFF7; // pin 3 GPIO controlled

    // TIMER
    // disable timer 3B
    TIMER3_CTL_R &= 0xFFFFFEFF;
    // use 16 bit timer
    TIMER3_CFG_R = TIMER_CFG_16_BIT; 
    // set TIMER3B input capture, edge-time mode, count down
    TIMER3_TBMR_R =  (TIMER3_TBMR_R | 0x107) & 0xFFFFFF107;
    // set TIMER3B to get both edges
    TIMER3_CTL_R |= 0xC00;
    // load max val for count down
    TIMER3_TBILR_R |= 0xFFFF;
    // load max val for prescaler
    TIMER3_TBPR_R |= 0xFF;
    // enable interrupts for timer B capture mode event interrupt only
    TIMER3_IMR_R = (TIMER3_IMR_R | 0x400) & 0xFFFFF400;
    // clear interrupt
    TIMER3_ICR_R |= 0x400;

    // bind interrupt to our handler
    NVIC_EN1_R |= 0x10;

    IntRegister(INT_TIMER3B, ping_interrupt_handler);
    // if we have issues with interrupt priority, include this:
//    IntMasterEnable();

    // enable timer 3B
    TIMER3_CTL_R |= 0x100;
}

void trigger_ping(){
    // disable timer 3B
    TIMER3_CTL_R &= 0xFFFFFEFF;
    // unmask interrupt
    TIMER3_IMR_R &= 0xFFFFFBFF;


    // set GPIO PB3 as output
    GPIO_PORTB_DIR_R |= 0x08;
    // make sure GPIO controlled
    GPIO_PORTB_AFSEL_R &= 0xFFFFFFF7;


    // send 20 microsecond pulse to activate sensor
    GPIO_PORTB_DATA_R |= 0x08;
    timer_waitMicros(20);
    GPIO_PORTB_DATA_R &= 0xFFFFFFF7;

    //clear Timer B interrupts
    TIMER3_ICR_R |= 0x400;
    // mask interrupt
    TIMER3_IMR_R = (TIMER3_IMR_R | 0x400) & 0xFFFFF4FF;
    // set GPIO PB3 as input and prepare to receive signal
    GPIO_PORTB_DIR_R &= 0xFFFFFFF7;
    GPIO_PORTB_AFSEL_R |= 0x08;
    // enable timer 3B
    TIMER3_CTL_R |= 0x100;
}

void ping_interrupt_handler(void) {
    if(TIMER3_MIS_R  & 0x400) {
        TIMER3_ICR_R |= 0x400;
        if(edgeCount == 0){
            timeRisingEdge = TIMER3_TBR_R;
            edgeCount++;
        }
        else if(edgeCount == 1) {
            timeFallingEdge = TIMER3_TBR_R;
            edgeCount = 2;
        }
    }
}

float delta_to_distance(int delta) {
   float clockSpeed = 6.25;
   float time = delta * clockSpeed;
   float distance = time * 34000;
   distance /= 100000000;
   return(distance/2);
}

int ping_read() {
    trigger_ping();
    while (edgeCount < 2);
    int delta = timeRisingEdge - timeFallingEdge;
    return delta_to_distance(delta);
}
