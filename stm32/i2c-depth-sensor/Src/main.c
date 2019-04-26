#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

#include "delay.h"
#include "usart.h"
#include "i2c.h"
#include "MS5803_I2C.h"

// PA2:		Tx
// PA3:		Rx
// PB10:	SCL
// PB11:	SDA

void setup() {
	DelayInit();
	// Initialize USART2
	USART2_Init();
	// Initialize I2C
	i2c_init();
	USART2_PutString("USART & I2C initialized\n");
	//MS5803_Init(ADDRESS_HIGH);
}

void depth_demo() {
	float temp, pres;
	temp = MS5803_GetTemperature(CELSIUS, ADC_256);
	pres = MS5803_GetPressure(ADC_4096);
	USART2_PutString("temp:");
	USART2_PutFloat(temp);
	USART2_PutString("\n");
	USART2_PutString("pressure:");
	USART2_PutFloat(pres);
	USART2_PutString("\n");
}
	
int main(void)
{
	setup();
	
	while (1)
	{
		//depth_demo();
		USART2_PutChar('a');
		DelayMs(2000);
	}
}

