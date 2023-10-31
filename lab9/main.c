#include "ping.h"
#include "lcd.h"

extern volatile int timeFallingEdge;
extern volatile int timeRisingEdge;
extern volatile int edgeCount;

int main(void)
{
    ping_init();
    lcd_init();
    trigger_ping();
    while (edgeCount < 2);
    int delta  = timeRisingEdge-timeFallingEdge;
    lcd_printf("rise: %d\nfall: %d\ndelta: %d", timeFallingEdge, timeRisingEdge, delta);
	return delta;
}
