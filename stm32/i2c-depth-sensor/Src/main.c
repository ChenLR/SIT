#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

#include "delay.h"
#include "usart.h"
#include "i2c.h"
#include "MS5803_I2C.h"
#include "rtc.h"

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
	
void RTC_Setup() {
  RTC_Stop();
  RTC_Set(DS1307_SEC,10);
  RTC_Set(DS1307_MIN,38);
  RTC_Set(DS1307_HR,17);
  RTC_Set(DS1307_DOW,5);
  RTC_Set(DS1307_DATE,26);
  RTC_Set(DS1307_MTH,4);
  RTC_Set(DS1307_YR,19);
  RTC_Start();
  //RTC.SetOutput(LOW);
  //RTC.SetOutput(HIGH);
  //RTC.SetOutput(DS1307_SQW1HZ);
  //RTC.SetOutput(DS1307_SQW4KHZ);
  //RTC.SetOutput(DS1307_SQW8KHZ);
  RTC_SetOutput(DS1307_SQW32KHZ);
}

int main(void)
{
	uint8_t rtc[7], i;
	setup();
	RTC_Setup();
	//RTC_SetOutput(DS1307_SQW32KHZ);
	//USART2_PutString("OutputType:");
	//USART2_PutInt(RTC_GetOutput());
	
	while (1)
	{
		//depth_demo();
		USART2_PutString("RTC get data\n");
		RTC_Get(rtc, 1);
		for(i = 0; i < 7; i++) {
			USART2_PutInt(rtc[i]);
			USART2_PutString("\n");
		}
		DelayMs(2000);
	}
}

