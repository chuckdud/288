/*
 * main.c
 *
 *  Created on: Sep 8, 2023
 *      Author: cddudley
 */
#include "movement.h"
#include "cyBot_uart.h"
#include "cyBot_Scan.h"
#include "Timer.h"
#include "lcd.h"
#include <math.h>


void ian() {

    oi_t *sensor_data = oi_alloc();
    timer_init();
    lcd_init();
    int init = 7;
    cyBot_uart_init();
    cyBOT_init_Scan(init);
//    cyBOT_SERVRO_cal_t calibration = cyBOT_SERVO_cal();
    left_calibration_value = 1193500;
    right_calibration_value = 248500;

    cyBOT_Scan_t data;


    int message = cyBot_getByte();

    if (message == 'm') {
        char toprint[50];
        char string[] = "\rDegrees\t\tDistance(cm)\n";

        int i = 0;
        while(i < strlen(string)){
            cyBot_sendByte(string[i]);
            i++;
        }
        int servoAngle;


        struct scanData{
            int dist;
        };
        struct object{
            int startingAngle;
            int width;
            double distance;
            int number;
        } ;

        struct object objects[5];
        struct scanData dataPoints[180];
        for (servoAngle = 0; servoAngle <= 180; servoAngle += 2) {
            cyBOT_Scan(servoAngle, &data);

            dataPoints[servoAngle].dist = data.sound_dist;
            sprintf(toprint, "\r%d\t\t%.1f\n", servoAngle, data.sound_dist);
            i = 0;
            while(i < strlen(toprint)){
                cyBot_sendByte(toprint[i]);
                i++;

            }

        }

        int original = 0;

        int numberObjects = 0;


        char output[] = "\rObject#\t\tAngle\t\tDistance\t\tWidth\n";

        int w = 0;
        while(w < strlen(output)){
            cyBot_sendByte(output[w]);
            w++;
        }


        while(original < 180){
            i = original + 2;
            if(dataPoints[original].dist >= 70){
                // no object
            }
            else if(abs((dataPoints[original].dist-dataPoints[i].dist))<=3){

                while(abs((dataPoints[i].dist-dataPoints[i+2].dist))<=3){

                    i=i+2;

                }
                if(abs(i-original)< 6){

                }
                else{
                    objects[numberObjects].startingAngle = original;
                    objects[numberObjects].distance = dataPoints[original].dist;
                    objects[numberObjects].number = numberObjects;
                    objects[numberObjects].width = abs(i-original) ;
                    numberObjects++;
                }
            }

            original = i;


        }

        int p = 0;
        //        while(p <= numberObjects -1){
        //            double DegreeToRadients = objects[p].width/57.295 / 2;
        //            double hb = cos(DegreeToRadients)*objects[p].distance;
        ////            objects[p].width = cosValue;
        ////              p++;
        //            objects[p].width = 2 * sqrt(objects[p].distance * objects[p].distance - hb*hb);
        //       }


        char toprintObject[50];
        int k = 0;
        while(k <= numberObjects-1){
            sprintf(toprintObject, "\r%d\t\t%d\t\t%f\t\t%d\n",objects[k].number+1,objects[k].startingAngle,objects[k].distance,objects[k].width);

            int u = 0;
            while(u <= strlen(toprintObject)){
                cyBot_sendByte(toprintObject[u]);

                u++;
            }

            k++;
        }
        int indexOfSmallest = 0;

        p = 0 ;
        while(p <= numberObjects -1){
            if(objects[p].width < objects[indexOfSmallest].width){
                indexOfSmallest = p;
            }
            p++;
        }

        cyBOT_Scan(objects[indexOfSmallest].startingAngle, &data);


        //      }
        //      lcd_putc(cyBot_getByte());
        //
        //    cyBot_sendByte(m);

        oi_free(sensor_data);
    }
}

