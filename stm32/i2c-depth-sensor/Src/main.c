#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

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


void setup() {
	DelayInit();
	USART1_Init();
	printf("Before Eink Init\n");
	Eink_Init();
	printf("After Eink Init\n");
	
	//ADC1_Init();
	//ds1307_init();
	//ms5803_Init(ADDRESS_HIGH);
	//led_7_seg_init();
}


int main(void)
{
	//float bat;
	setup();
	Eink_demo();
	while (1)
	{
		//led_7_seg_demo();
		//ms5803_demo();
		//bat = ADC1_ReadBattery();
		//printf("%f\n", bat);
		//ds1307_demo();
		printf("xxx");
		DelayMs(2000);
	}
}

