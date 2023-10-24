

/**
 * main.c
 */
#include "movement.h"
#include "uart.h"
#include "cd.h"
#include "open_interface.h"
#include "cyBot_Scan.h"
#include "stdlib.h"

void main()
{

    oi_t *sensor = oi_alloc();
    timer_init();
    lcd_init();
    int init = 7;
    uart_init(115200);
    cyBOT_init_Scan(init);

//    cyBOT_SERVRO_cal_t calibration = cyBOT_SERVO_cal();
//    right_calibration_value = calibration.right;
//    left_calibration_value = calibration.left;

    right_calibration_value = 274750;
    left_calibration_value = 1256500;
    while(1){
        char data = uart_receive();
        if(data == 'w'){
            move_forward(sensor,100);
        }
        else if(data == 's'){
            move_backward(sensor,100);
        }
        else if(data == 'a'){
            turn_counter_clockwise(sensor, 15);
        }
        else if(data == 'd'){
            turn_clockwise(sensor,15);
        }
        else if(data == 'm'){
            uart_sendStr("\rDegrees\t\tDistance(cm)\n");
            point_t points[91];
            scan_180(points);
            print_180(points);
            find_smallest_object(points);
        }
        else if (data == 'q') break;
    }
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
