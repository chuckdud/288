

/**
 * main.c
 */
#include "movement.h"
#include "uart.h"
#include "cd.h"
#include "open_interface.h"
#include "cyBot_Scan.h"
#include "stdlib.h"
extern volatile char flag;
void main()
{

    oi_t *sensor = oi_alloc();
    timer_init();
    lcd_init();
    int init = 7;
    uart_init(115200);
    cyBOT_init_Scan(init);
    uart_interrupt_init();
    cyBOT_SERVRO_cal_t calibration = cyBOT_SERVO_cal();
    right_calibration_value = calibration.right;
    left_calibration_value = calibration.left;

//    right_calibration_value = 259000;
//    left_calibration_value = 1246000;
    point_t points[91];
    struct object objects[4];
    while(1){
        if (flag == 'm') {
            flag = 0;
            while (flag != 'h') {
                if(flag == 'w'){
                    move_forward(sensor,100);
                }
                else if(flag == 's'){
                    move_backward(sensor,100);
                }
                else if(flag == 'a'){
                    turn_counter_clockwise(sensor, 15);
                }
                else if(flag == 'd'){
                    turn_clockwise(sensor,15);
                }
                else if(flag == 'm'){
                    scan_180(points);
                    print_180(points);

                    find_smallest_object(points, objects);
                    timer_waitMicros(1000);
                    scan_Ping(objects);
                }
                flag = 0;
                timer_waitMicros(1000);
            }
        }
        if (flag == 'h'){
            while (flag != 'm') {
                flag = 0;
                scan_180(points);
                print_180(points);
                find_smallest_object(points, objects);
                timer_waitMicros(1000);
                scan_Ping(objects);
                while (flag != 'h') {
                    if (flag == 'm') break;
                    timer_waitMicros(1000);
                }
                if (flag == 'h') driveToSmallest(sensor,objects);
                timer_waitMicros(1000);
            }
        }
        if (flag == 'q') break;
    }
    oi_free(sensor);
}
