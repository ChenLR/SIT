#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

#include "delay.h"
#include "usart.h"
#include "i2c.h"
#include "MS5803_I2C.h"

#define SLAVE_ADDRESS		0x08



uint8_t receivedByte;
	
int main(void)
{
	DelayInit();
	
	// Initialize USART2
	USART2_Init();
	
	// Initialize I2C
	i2c_init();
	
	USART2_PutString("USART & I2C initialized\n");
	MS5803_Init(ADDRESS_HIGH);
	
	
	while (1)
	{
		float temp = 0.1, pres;
		temp = MS5803_GetTemperature(CELSIUS, ADC_256);
		pres = MS5803_GetPressure(ADC_4096);
		USART2_PutString("temp:");
		USART2_PutFloat(temp);
		USART2_PutString("\n");
		USART2_PutString("pressure:");
		USART2_PutFloat(pres);
		USART2_PutString("\n");
		DelayMs(2000);
		// USART2_PutChar('z');
		
		/*
		// Write 0x01 to slave (turn on LED blinking)
		i2c_write(SLAVE_ADDRESS, 0x01);
		DelayMs(5);
		// Read LED blinking status (off/on)
		i2c_read(SLAVE_ADDRESS, &receivedByte);
		
		// Display LED blinking status
		lcd16x2_clrscr();
		
		if (receivedByte == 0)
		{
			lcd16x2_puts("LED Blinking Off");
		}
		else if (receivedByte == 1)
		{
			lcd16x2_puts("LED Blinking On");
		}
		DelayMs(2500);
		
		// Write 0x00 to slave (turn off LED blinking)
		i2c_write(SLAVE_ADDRESS, 0x00);
		DelayMs(5);
		// Read LED blinking status (off/on)
		i2c_read(SLAVE_ADDRESS, &receivedByte);
		// Display LED blinking status
		lcd16x2_clrscr();
		if (receivedByte == 0)
		{
			lcd16x2_puts("LED Blinking Off");
		}
		else if (receivedByte == 1)
		{
			lcd16x2_puts("LED Blinking On");
		}
		DelayMs(2500);
		*/
	}
}

