#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

#include "delay.h"
#include "usart.h"
#include "i2c.h"
#include "ms5803.h"
#include "ds1307.h"

// PA2:		Tx
// PA3:		Rx
// PB10:	SCL
// PB11:	SDA



void setup() {
	DelayInit();
	USART2_Init();
	ds1307_init();
	ms5803_Init(ADDRESS_HIGH);
}


int main(void)
{
	setup();
	
	while (1)
	{
		ms5803_demo();
		ds1307_demo();
		DelayMs(2000);
	}
}

