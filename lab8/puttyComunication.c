

/**
 * main.c
 */
#include "movement.h"
#include "uart.h"
#include "cd.h"
#include "open_interface.h"
#include "cyBot_Scan.h"
#include "stdlib.h"
#include "adc.h"
#include "lcd.h"
#include "Timer.h"
extern volatile char flag;
void main()
{

    oi_t *sensor = oi_alloc();
    timer_init();
    lcd_init();
    int init = 7;
    uart_init(115200);

    cyBOT_init_Scan(init);
//    uart_interrupt_init();
    adc_init();

/*
    char distance[100];
    cyBOT_Scan_t data;

    while (1)
    {
        if (uart_receive() == 'r')
        {
            cyBOT_Scan(90, &data);
            int reading = adc_read();
            double dist = -7.79994 * pow(10, -9) * pow(reading, 3) + 0.0000540086 * pow(reading, 2) - 0.126712 * reading + 116.479;
            sprintf(distance, "\n\rADC Value:%d\t\t Estimated Distance:%f.1", reading, dist);
            uart_sendStr(distance);
        }
    }
    cyBOT_SERVRO_cal_t calibration = cyBOT_SERVO_cal();
    right_calibration_value = calibration.right;
    left_calibration_value = calibration.left;
*/

    right_calibration_value = 269500;
    left_calibration_value = 1251250;
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
    }
    oi_free(sensor);
}
