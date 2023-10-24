/**
 * lab5_template.c
 * 
 * Template file for CprE 288 lab 5
 *
 * @author Zhao Zhang, Chad Nelson, Zachary Glanz
 * @date 08/14/2016
 *
 * @author Phillip Jones, updated 6/4/2019
 */

#include "button.h"
#include "timer.h"
#include "lcd.h"
#include "math.h"

#include "cyBot_uart.h"  // Functions for communiticate between CyBot and Putty (via UART)
// PuTTy: Buad=115200, 8 data bits, No Flow Control, No Party,  COM1

#include "cyBot_Scan.h"  // For scan sensors


//#warning "Possible unimplemented functions"
//#define REPLACEME 0


// Defined in button.c : Used to communicate information between the
// the interupt handeler and main.
extern volatile int button_event;
extern volatile int button_num;


int get_avg_10() {
    cyBOT_Scan_t data;
    int i;
    int sum = 0;
    for (i = 0; i < 10; i++) {
        cyBOT_Scan(90, &data);
        sum = sum + data.IR_raw_val;
    }
    return sum / 10;
}

int raw_to_dist(int raw) {
    //    y = 11.29948 + (188820000 - 11.29948)/(1 + (x/15.11238)^3.667047)
    return  11.29948 + (188820000 - 11.29948) / (1 + pow(raw / 15.11238, 3.667047));
}

int main(void) {
    button_init();
    lcd_init();

    init_button_interrupts();
    left_calibration_value = 1251250;
    right_calibration_value = 259000;
//    cyBOT_Scan_t data;

    cyBot_uart_init_clean();  // Clean UART initialization, before running your UART GPIO init code

    // Complete this code for configuring the  (GPIO) part of UART initialization
    SYSCTL_RCGCGPIO_R |= 0x02;
    timer_waitMillis(1);            // Small delay before accessing device after turning on clock
    GPIO_PORTB_AFSEL_R |= 0x02;
    GPIO_PORTB_PCTL_R &= 0x00;     // Force 0's in the disired locations
    GPIO_PORTB_PCTL_R |= 0x10;     // Force 1's in the disired locations
    GPIO_PORTB_DEN_R |=  0x02;
    GPIO_PORTB_DIR_R &= 0x0;      // Force 0's in the disired locations
    GPIO_PORTB_DIR_R |= 0x02;      // Force 1's in the disired locataions

    cyBot_uart_init_last_half();  // Complete the UART device initialization part of configuration

    // Initialze scan sensors
    cyBOT_init_Scan(7);

    // pretty sure this function is broken in new uart library
    //cyBot_getByte_blocking();

    while(1){
//        cyBOT_Scan(90, &data);
        int avg = get_avg_10();
        int dist = raw_to_dist(avg);

        char toPrint[20];
        sprintf(toPrint,"%d\n%d\n", avg, dist);
        lcd_printf(toPrint);



        // constantly reading data and writing to putty.log in this project
        // can easily use this to collect data. Might have to
        /**
         * constantly reading data and writing to putty.log in this project
         *
         * can easily use this to set something Xcm away, take a bunch of measurements, import to excel
         *  repeat with another distance as necessary
         */
//        int i = 0;
//
//        while(i < strlen(toPrint)){
//            cyBot_sendByte(toPrint[i]);
//            i++;
//        }
    }
    // YOUR CODE HERE

    //	while(1)
    //	{
    //	    lcd_clear();
    //        char button;
    //        char toPrint[4] ;
    //        while(1)
    //        {
    //         button = button_getButton();
    //         sprintf(toPrint,"%d\n\r",button);
    //         int i = 0;
    //         while(i < strlen(toPrint)){
    //             cyBot_sendByte(toPrint[i]);
    //             i++;
    //         }
    //
    //      // YOUR CODE HERE
    //
    //
    //        }
    //
    //	}






    //
    //	while (1) {
    //	    if (button_event) {
    //
    //	        sprintf(toPrint,"%d\n\r",button_num);
    //	        int i = 0;
    //             while(i < strlen(toPrint)){
    //                 cyBot_sendByte(toPrint[i]);
    //                 i++;
    //             }
    //	        button_event = 0;
    //	    }
    //	}

}
