#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "delay.h"
#include "wkup.h"

void delay(unsigned int nCount);

GPIO_InitTypeDef GPIO_InitStruct;

int main (void)
{	
	DelayInit();
	WKUP_Init();
	// Enable clock for GPIOC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	// Configure PA13 as push-pull output
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	while (1)
	{
		/* Toggle LED on PA13 */
		// Reset bit will turn on LED (because the logic is interved)
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		DelayMs(500);
		// Set bit will turn off LED (because the logic is interved)
		GPIO_SetBits(GPIOC, GPIO_Pin_13);
		DelayMs(500);
	}
}

