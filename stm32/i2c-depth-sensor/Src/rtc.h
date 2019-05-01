/*
  DS1307.h - library for DS1307 rtc
*/

// ensure this library description is only included once
#ifndef __RTC_H
#define __RTC_H

#include "stm32f10x.h"


#define DS1307_SEC 0
#define DS1307_MIN 1
#define DS1307_HR 2
#define DS1307_DOW 3
#define DS1307_DATE 4
#define DS1307_MTH 5
#define DS1307_YR 6

#define DS1307_BASE_YR 2000

#define DS1307_SQW1HZ  7
#define DS1307_SQW4KHZ 8
#define DS1307_SQW8KHZ 9
#define DS1307_SQW32KHZ 10

#define DS1307_CTRL_ID 0x68  //DS1307  1101000

 // Define register bit masks
#define DS1307_CLOCKHALT 0x80    // 0b10000000

#define DS1307_LO_BCD  0x0F
#define DS1307_HI_BCD  0xF0

#define DS1307_HI_SEC  0x70    // 0b01110000
#define DS1307_HI_MIN  0x70    // 0b01110000
#define DS1307_HI_HR   0x30    // 0b00110000
#define DS1307_LO_DOW  0x07    // 0b00000111
#define DS1307_HI_DATE 0x30    // 0b00110000
#define DS1307_HI_MTH  0x30    // 0b00110000
#define DS1307_HI_YR   0xf0    // 0b11110000


// Prescaler
#define DS1307_LOW_BIT		0x00    // b00000000
#define DS1307_HIGH_BIT		0x80    // b10000000
#define DS1307_SQW1HZ_BIT	0x10    // b00010000
#define DS1307_SQW4KHZ_BIT	0x11    // b00010001
#define DS1307_SQW8KHZ_BIT	0x12    // b00010010
#define DS1307_SQW32KHZ_BIT	0x13    // b00010011

// library interface description
  // user-accessible "public" interface
void RTC_Get(uint8_t *, uint8_t);
int RTC_GetIndividual(uint8_t c, uint8_t refresh);
//int RTC_Get(int, int);
void RTC_Set(int, int);
void RTC_Start(void);
void RTC_Stop(void);
void RTC_SetOutput(int c);
int RTC_GetOutput(void);


#endif
