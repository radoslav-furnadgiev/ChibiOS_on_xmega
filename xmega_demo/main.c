/*
 * PortTest.c
 *
 * Created: 5/16/2012 2:44:07 PM
 *  Author: swinchen
 */ 

#include <ch.h>
#include <hal.h>
#include <test.h>

#include <avr/io.h>


WORKING_AREA(waThread1, 64);
WORKING_AREA(waThread2, 64);

msg_t vThread1(void* arg) {
	volatile unsigned short int i;
	systime_t time = chTimeNow();
	for(;;) {
		time += MS2ST(20);
		i = i;
		chThdSleepUntil(time);
	}
        return 0;
}

msg_t vThread2(void* arg) {
	volatile unsigned short int i;
	systime_t time = chTimeNow();
	for(;;) {
		time += MS2ST(10);
		i = i;
		chThdSleepUntil(time);
	}
        return 0;
}


int main(void)
{
	halInit();
	chSysInit();

	sdStart(&SDC1, NULL);

	TestThread(&SDC1);
	
	chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, vThread1, 0);
	chThdCreateStatic(waThread2, sizeof(waThread2), NORMALPRIO, vThread2, 0);
	
    while(1)
    {
		chThdSleepMilliseconds(1000);
        //TODO:: Please write your application code 
    }
}
