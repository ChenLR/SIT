#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

#include "wkup.h"
#include "delay.h"
#include "usart.h"
#include "i2c.h"
#include "ms5803.h"
#include "ds1307.h"
#include "led_display.h"
#include "adc.h"
#include "eink.h"

// PA9:		Tx
// PA10:	Rx
// PB10:	SCL
// PB11:	SDA


void Device_Setup() {
	// wake up if pressed more than 2s
	DelayInit();
	WKUP_Init();
	
	// initialize peripherals
	USART1_Init();
	printf("##################\n");
	printf("USART1 Initialized\n");
	Eink_Init();
	printf("Eink Display Initialized\n");
	ADC1_Init();
	printf("ADC1 Initialized\n");
	ds1307_init();
	printf("RTC Initialized\n");
	ms5803_Init(ADDRESS_HIGH);
	printf("Depth Sensor Initialized\n");
	LED_7_Seg_Init();
	printf("LED Display Initialized\n");
	printf("------------------\n");
	printf("All Peripherals Initialized\n");
	printf("------------------\n");
	
	// register standby functions for peripherals
	Register_Standby_Funcs(LED_7_Seg_Standby);
	// Register_Standby_Funcs(Eink_Standby);
}


int main(void)
{
	int cnt_100ms = 0;
	float depth = 99.99;
	
	Device_Setup();

	while (1)
	{
		if(!(cnt_100ms % 5)) Toggle_LED_Green(); // 0.5s
		if(!(cnt_100ms % 20)) ds1307_demo();		// 1s
		if(!(cnt_100ms % 20)) ms5803_demo();		// 1s
		if(!(cnt_100ms % 20)) Battery_demo();		// 2s
		if(!(cnt_100ms % 20)) LED_7_Seg_Demo();	// 2s
		if(!(cnt_100ms % 1200)) Eink_demo();			// 120s
		
		if(!(cnt_100ms % 6000)) cnt_100ms = 0;

		cnt_100ms++;
		DelayMs(100);
	}
}

