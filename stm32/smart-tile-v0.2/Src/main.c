#include "main.h"
#include "wkup.h"
#include "delay.h"
#include "usart.h"
#include "i2c.h"
#include "ms5803.h"
#include "adc.h"
#include "eink.h"
#include "fonts.h"
#include "rtc.h"

// PA9:		Tx
// PA10:	Rx
// PB10:	SCL
// PB11:	SDA
extern int RECV_CNT;
extern int dbg_line_flag;
extern char dbg_line[];


void deviceSetup() {
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
	RTC_Init();
	printf("RTC Initialized\n");
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

void printSensorData(float pressure, float depth, float battery) {
	printf("--------\n");
	printf("pressure  %.1f\n", pressure);
	printf("Vbatt     %.4fV\n", battery);
	printf("received  %d\n", RECV_CNT);
	RTC_Get();
	printf("Alarm Time:%d-%d-%d %d:%d:%d\n",calendar.w_year,calendar.w_month,
	calendar.w_date,calendar.hour,calendar.min,calendar.sec);
	if(dbg_line_flag) printf("package  %s\n", dbg_line);
}

void einkUserLogic(float pressure, float depth, float battery) {
	static uint8_t welcome_flag = 0;
	if(welcome_flag) {
		welcome_flag = 0;
		Eink_Display_Welcome(pressure, depth, battery);
	}
	else {
		char line[30];
		ClearBuffer();
		DrawStringAt(0, 0, "SIT  Debugging", &Font24, 1.2, 1);
		sprintf(line, "Recv %d", RECV_CNT);
		DrawStringAt(0, 40, line, &Font24, 1, 1);
		sprintf(line, "Pressure  %.1f", pressure);
		DrawStringAt(0, 70, line, &Font24, 1, 1);
		//sprintf(line, "Vbatt %.4fV", battery);
		if(dbg_line_flag)  DrawStringAt(0, 100, dbg_line, &Font8, 1, 1);
		Eink_SetAndDisplay();
	}
}

int main(void)
{
	// clock for tasks
	int cnt_100ms = 0;
	
	// sensor data
	float pressure = -1.11, depth = -1.11, battery = -1.11;
	
	// initialization
	deviceSetup();

	while (1)
	{
		// update data
		if(!(cnt_100ms % PERIOD_LED)) Toggle_LED_Green();
		if(!(cnt_100ms % PERIOD_DEPTH)) ms5803_getDepthAndPressure(&depth, &pressure);
		if(!(cnt_100ms % PERIOD_BATT)) battery = ADC1_ReadBattery();
		// transmit
		if(!(cnt_100ms % PERIOD_PRINT_SENSOR)) printSensorData(pressure, depth, battery);
		// display
		if(cnt_100ms % PERIOD_EINK == PERIOD_EINK - 20) {
			// 2s before display digits
			// clear the screen to prevent from burning
			Eink_ClearFrameMemory(0xFF);
			Eink_DisplayFrame();
		}
		if(!(cnt_100ms % PERIOD_EINK)) {
			einkUserLogic(pressure, depth, battery);
		}
		// prevent overflow
		if(!(cnt_100ms % PERIOD_OVERALL)) cnt_100ms = 0;

		cnt_100ms++;
		DelayMs(100);
	}
}

