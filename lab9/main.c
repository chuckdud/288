#include "ping.h"
#include "lcd.h"

extern volatile int timeFallingEdge;
extern volatile int timeRisingEdge;
extern volatile int edgeCount;

int main(void)
{
    ping_init();
    lcd_init();
    while (1) {
        trigger_ping();
        while (edgeCount < 2);
        edgeCount = 0;
        int delta  = timeRisingEdge-timeFallingEdge;
//        lcd_printf("rise: %d\nfall: %d\ndelta: %d", timeFallingEdge, timeRisingEdge, delta);
        lcd_printf("delta: %d cycles\ndistance: %.2f cm", delta, deltaToDistance(delta));
        timer_waitMillis(1000);
    }
//	return delta;
}
