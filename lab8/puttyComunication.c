

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
    uart_interrupt_init();
    adc_init();
//    char distance[100];
//    cyBOT_Scan_t data;

//    while(1){
//    if(uart_receive() == 'r'){
//    cyBOT_Scan(90, &data);
//    int reading = adc_read();
//    double dist = -7.79994*pow(10,-9)*pow(reading,3)+0.0000540086*pow(reading,2)-0.126712*reading +116.479;
//    sprintf(distance,"\n\rADC Value:%d\t\t Estimated Distance:%f.1",reading,dist);
//    uart_sendStr(distance);
//    }
//    }
     //   cyBOT_SERVRO_cal_t calibration = cyBOT_SERVO_cal();
    //    right_calibration_value = calibration.right;
    //    left_calibration_value = calibration.left;

    right_calibration_value = 269500;
    left_calibration_value = 1251250;
    point_t points[91];
    struct object objects[4];
    while(1){
        if (flag == 'm'){
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
                driveToSmallest(sensor,objects);
                while (flag != 'h') {
                    if (flag == 'm') break;
                    timer_waitMicros(1000);
                }
                /**
                 * right now the number of objects is never cleared, so if we run a manual scan at some time earlier and then run an autonomous scan at some time later, old objects will still be there.
                 *
                 * very close to being done, just need to figure out a better way to clear old objects?
                 */
                if (flag == 'h') driveToSmallest(sensor,objects);
                flag = 0;
                timer_waitMicros(1000);
            }
        }
    }






    //        else if (data == 'q') break;
    //    }
    //    char m;
    //    m = 'M';
    //    char string [] ="Got an m";
    //    int message = cyBot_getByte();
    //    if(message == m){
    //        int i = 0;
    //        while(i < strlen(string)){
    //            cyBot_sendByte(string[i]);
    //                    i++;
    //        }
    //    }
    //lcd_putc(cyBot_getByte());
    //    cyBot_sendByte(m);
    oi_free(sensor);
}
