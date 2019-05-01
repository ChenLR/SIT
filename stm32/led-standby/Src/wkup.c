#include "wkup.h"
#include "delay.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_exti.h"

//1. In run mode, when PA0 occurs a rising edge, the interrupt routine of PA0 
//   will execute and check if pressing time is longer than 2s, then turn into standby mode.
//2. In standby mode, when PA0 occurs a rising edge, the program start from beginning. 
//   At the end of initialization, check if pressing time is longer than 2s, if not, back to standby mode,
//   otherwise, continue to run.

//enter standby mode
void Sys_Standby(void)
{  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	//enable PWR clock
	PWR_WakeUpPinCmd(ENABLE);  //enable PA0 wakeup pin
	PWR_EnterSTANDBYMode();	  //enter standby mode 
}

void Sys_Enter_Standby(void)
{			 
	RCC_APB2PeriphResetCmd(0x01fc,DISABLE);//reset
	Sys_Standby();
}


//check if it is a real wakeup or noise	
uint8_t Check_WKUP(void) 
{
	uint8_t t=0;	//record time of pressing
	while(1)
	{ 
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0))
		{
			t++;			
			DelayMs(20);
			if(t>=100)		//press time is enough
			{
				//LED0=0;	 	
				return 1; 	
			}
		}else 
		{ 
			//LED0=1;
			return 0; 
		}
	}
}  


//PA0 interrupt
void EXTI0_IRQHandler(void)
{ 		    		    				     		    
	EXTI_ClearITPendingBit(EXTI_Line0); //clear interrupt  
	if(Check_WKUP())//check if it is a real turn off
	{		  
		Sys_Enter_Standby();  
	}
} 

//initialize wakeup module
void WKUP_Init(void)
{	
  GPIO_InitTypeDef GPIO_InitStructure;  		  
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
 
	//initialize gpio
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0;	 
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPD;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);	
 
	//initialize external interrupt
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;	
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //trigger at rising edge
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	
 
	// Set interrupt priority group
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	// Set System Timer IRQ at higher priority
	NVIC_SetPriority(SysTick_IRQn, 0);
	// Set PA0 priority
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure); 
 
	if(Check_WKUP()==0) Sys_Standby(); //if not press 2s, still standby
		
}
