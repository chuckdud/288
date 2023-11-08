/*
*
*   uart_extra_help.c
* Description: This is file is meant for those that would like a little
*              extra help with formatting their code, and followig the Datasheet.
*/

#include "uart.h"
#include "Timer.h"
#define REPLACE_ME 0x00


void uart_init(int baud)
{
    SYSCTL_RCGCGPIO_R |= 0b10;      // enable clock GPIOB (page 340)
    SYSCTL_RCGCUART_R |= 0b10;      // enable clock UART1 (page 344)
    GPIO_PORTB_AFSEL_R = 0b11;      // sets PB0 and PB1 as peripherals (page 671)
    GPIO_PORTB_PCTL_R  = (GPIO_PORTB_PCTL_R | 0b10001) & 0b10001; // pmc0 and pmc1       (page 688)  also refer to page 650
    GPIO_PORTB_DEN_R   = 0b11;        // enables pb0 and pb1
    GPIO_PORTB_DIR_R   = (GPIO_PORTB_DIR_R | 0b01) & 0b01; // sets pb0 as output, pb1 as input

    //compute baud values [UART clock= 16 MHz] 
    double fbrd;
    int    ibrd;

    fbrd = ((double) 16000000) / (16 * baud);
    ibrd = (16000000) / (16 * baud);
    fbrd = (int)((fbrd - (double)ibrd) * 64 + 0.5);

    UART1_CTL_R &= 0xFFFFFFFE;      // disable UART1 (page 918)
    UART1_IBRD_R = ibrd;        // write integer portion of BRD to IBRD
    UART1_FBRD_R = fbrd;   // write fractional portion of BRD to FBRD
    UART1_LCRH_R = (UART1_LCRH_R | 0x00000060) & 0xFFFFFF6F;        // write serial communication parameters (page 916) * 8bit and no parity
    UART1_LCRH_R = 0x60;        // write serial communication parameters (page 916) * 8bit and no parity
    UART1_CC_R   = 0b0;          // use system clock as clock source (page 939)
    UART1_CTL_R |= 0x1;        // enable UART1

}

void uart_sendChar(char data)
{
    while (UART1_FR_R & 0x20);
    UART1_DR_R = data;
}

char uart_receive(void)
{
    uint32_t ret;
    char rdata;
    while ((UART1_FR_R & 0x10) != 0);
    ret = UART1_DR_R;
//    if (ret & 0xF00) {
//        // error occurred
//    } else {
    rdata = (char)(ret & 0xFF);
//    }
    return rdata;
}

void uart_sendStr(const char *data)
{
    while (*data) {
        uart_sendChar(*data);
        data++;
    }
}

void uart_interrupt_init()
{
    // Enable interrupts for receiving bytes through UART1
    UART1_IM_R |= 0x10; //enable interrupt on receive - page 924

    // Find the NVIC enable register and bit responsible for UART1 in table 2-9
    // Note: NVIC register descriptions are found in chapter 3.4
    NVIC_EN0_R |= 0b1000000; //enable uart1 interrupts - page 104

    // Find the vector number of UART1 in table 2-9 ! UART1 is 22 from vector number page 104
    IntRegister(INT_UART1, uart_interrupt_handler); //give the microcontroller the address of our interrupt handler - page 104 22 is the vector number

}

void uart_interrupt_handler()
{
    if (UART1_MIS_R & 0x10)
    {
//        flag = UART1_DR_R & 0xFF;
    }

//    UART1_ICR_R &= 0xFFFFFFEF;
}
