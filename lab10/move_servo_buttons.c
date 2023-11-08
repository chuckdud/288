#include "servo.h"
#include "button.h"
#include "lcd.h"
extern volatile int falling_edge;

char degrees[20];

void move_n_display_servo(int degree) {
    servo_move(degree);
    sprintf(degrees, "%d", falling_edge);
    lcd_clear_puts(degrees);
}

int move_servo_buttons(void)
{
    button_init();
    init_button_interrupts();
    lcd_init();
    int degree = 90;
    int direction = 1;
    servo_init();
    servo_move(90);
    while (1)
    {
        int button = button_getButton();
        if (button == 1)
        {
            degree += (1 * direction);
            move_n_display_servo(degree);
        }
        else if (button == 2)
        {
            degree += (5 * direction);
            move_n_display_servo(degree);
        }
        else if (button == 3)
        {
            direction *= -1;
            if (direction < 0) lcd_clear_puts("CW");
            else lcd_clear_puts("CCW");
            timer_waitMillis(500); // prevent double switching
        }
        else if (button == 4)
        {
            if (direction > 0)
            {
                lcd_clear_puts("Moving Counter Clockwise");
                degree = 0;
                move_n_display_servo(degree);
            }
            else
            {
                lcd_clear_puts("Moving Clockwise");
                degree = 180;
                move_n_display_servo(degree);
            }
        }
    }
    return 0;
}
