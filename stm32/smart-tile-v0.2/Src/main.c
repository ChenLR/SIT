#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

#include "wkup.h"
#include "delay.h"
#include "usart.h"
#include "i2c.h"
#include "ms5803.h"
#include "adc.h"
#include "eink.h"

// PA9:		Tx
// PA10:	Rx
// PB10:	SCL
// PB11:	SDA


void Device_Setup() {
	// wake up if pressed more than 2s
	DelayInit();
	Init_LED_Green();
	WKUP_Init();
	
	// initialize peripherals
	USART1_Init();
	printf("##################\n");
	printf("USART1 Initialized\n");
	USART3_Init();
	printf("USART3 Initialized\n");
	Eink_Init();
	printf("Eink Display Initialized\n");
	ADC1_Init();
	printf("ADC1 Initialized\n");
	ms5803_Init(ADDRESS_HIGH);
	printf("Depth Sensor Initialized\n");
	printf("------------------\n");
	printf("All Peripherals Initialized\n");
	
	// register standby functions for peripherals
	Register_Standby_Funcs(Eink_Standby);
}

void print_sensor_data(float pressure, float depth, float battery) {
	printf("--------\n");
	printf("pressure  %.1f\n", pressure);
	printf("Vbatt     %.4fV\n", battery);
}

static uint8_t welcome_flag = 1;

int main(void)
{
	int cnt_100ms = 0;
	
	// sensor data
	float pressure = -1.11, depth = -1.11, battery = -1.11;
	
	Device_Setup();

	while (1)
	{
		// update data
		if(!(cnt_100ms % 5)) Toggle_LED_Green(); // 0.5s
		if(!(cnt_100ms % 10)) ms5803_getDepthAndPressure(&depth, &pressure);    // 1s
		if(!(cnt_100ms % 10)) battery = ADC1_ReadBattery();                     // 1s
		// display or transmit
		if(!(cnt_100ms % 30)) print_sensor_data(pressure, depth, battery);      // 3s
		if(!(cnt_100ms % 100)) {		                                            // 10s
			if(welcome_flag) {
				welcome_flag = 0;
				Eink_Display_Welcome(pressure, depth, battery);
			}
			else Eink_Display_Depth(depth);
		}
		// prevent overflow
		if(!(cnt_100ms % 6000)) cnt_100ms = 0;

		cnt_100ms++;
		DelayMs(100);
	}
}

