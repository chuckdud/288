/**
 * lab6_template_extra_help.c
 *
 * Description: This is file is meant for those that would like a little
 *              extra help with formatting their code.
 *
 */

#define _RESET 1
#define _PART1 0
#define _PART2 0
#define _PART3 0

#include "uart.h"
#include <stdint.h>
#include <stdbool.h>
#include "timer.h"
#include "lcd.h"
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"



//#if _RESET
//#include "Simulation_User Files_resetSimulation.h"
//#endif

// Adding global volatile varibles for communcating between 
// your Interupt Service Routine, and your non-interupt code.

volatile  char uart_data;  // Your UART interupt code can place read data here
volatile  char flag;       // Your UART interupt can update this flag
                           // to indicate that it has placed new data
                           // in uart_data                     


void tmplt()
{
//#if _RESET
//    resetSimulationBoard();
//#else

    timer_init();
    lcd_init();
    uart_init(115200);
    uart_interrupt_init();

////#if _PART1      // Receive and display text
//    // TODO
//	int chars_read = 0;
//	char data[20];
//	while (chars_read < 20) {
//	    char tmp = uart_receive();
//	    if (tmp == 13) break;
//	    data[chars_read] = tmp;
//	    chars_read++;
//	}
//	char to_print[21];
//	sprintf(to_print, "%s", data);
//	lcd_printf(to_print);
//
////#endif

////#if _PART2      // Echo Received Character
//	// TODO
//	char data[50];
//	int chars_transmitted = 0;
//	while (chars_transmitted < 50) {
//	    data[chars_transmitted] = uart_receive();
//	    uart_sendChar(data[chars_transmitted]);
//	    chars_transmitted++;
//	}
////#endif

#if _PART3 // Receive and send characters using interrupts.
 // TODO
	while (1);

#endif

//#endif
}

