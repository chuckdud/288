/*
 * simple_mission.c
 *
 *  Created on: Nov 8, 2023
 *      Author: cddudley
 */

#include "movement.h"
#include "uart.h"
#include "ping.h"
#include "scan.h"
#include "servo.h"
#include "adc.h"
#include "lcd.h"
#include "open_interface.h"
#include "stdlib.h"
#include "Timer.h"

//extern volatile char flag;

void main()
{

    oi_t *sensor = oi_alloc();
    timer_init();
    lcd_init();
    int init = 7;
    uart_init(115200);
    servo_init();
    //    uart_interrupt_init();
    adc_init();
    ping_init();

    point_t points[91];
    struct object objects[4];
    char command_char;
    while (1)
    {
        command_char = uart_receive();
        if (command_char == 'm')
        {
            uart_sendStr("entering manual mode\n");
            while (command_char != 'h')
            {
                command_char = uart_receive();
                if (command_char == 'w')
                {
                    uart_sendStr("moving forward\n");
                    move_forward(sensor, 100);
                }
                else if (command_char == 's')
                {
                    uart_sendStr("moving backward\n");
                    move_backward(sensor, 100);
                }
                else if (command_char == 'a')
                {
                    uart_sendStr("turning clockwise\n");
                    turn_counter_clockwise(sensor, 15);
                }
                else if (command_char == 'd')
                {
                    uart_sendStr("turning counterclockwise\n");
                    turn_clockwise(sensor, 15);
                }
                //                else if (command_char == 'm')
                //                {
                //                    uart_sendStr("scanning\n");
                //                    scan_180(points);
                //                    print_180(points);
                //
                //                    find_smallest_object(points, objects);
                //                    timer_waitMicros(1000);
                //                    scan_Ping(objects);
                //                }
            }
        }
        if (command_char == 'h')
        {
            uart_sendStr("entering autonomous mode\n");
            while (command_char != 'm')
            {
                scan_180(points);
                print_180(points);
                find_smallest_object(points, objects);
                timer_waitMicros(1000);
                scan_Ping(objects);
                driveToSmallest(sensor, objects);
                while (command_char != 'h')
                {
                    if (command_char == 'm')
                        break;
                    timer_waitMicros(1000);
                }
                if (command_char == 'h')
                    driveToSmallest(sensor, objects);
                command_char = uart_receive();
            }
        }
        if (command_char == 'q') break;
    }
    oi_free(sensor);
}

