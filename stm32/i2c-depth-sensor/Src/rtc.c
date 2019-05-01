#include "i2c.h"
#include "rtc.h"
#include "usart.h"

// PRIVATE FUNCTIONS
// library-accessible "private" interface
uint8_t rtc_bcd[7]; // used prior to read/set ds1307 registers;
void RTC_Read(void);
void RTC_Save(void);
void RTC_SendCommand(uint8_t command);



// Aquire data from the RTC chip in BCD format
// refresh the buffer
void RTC_Read(void)
{
  // use the Wire lib to connect to tho rtc
  // reset the resgiter pointer to zero
  //Wire.beginTransmission(DS1307_CTRL_ID);
  //Wire.send(0x00);
  //Wire.endTransmission();
	RTC_SendCommand(0x00);

  // request the 7 bytes of data    (secs, min, hr, dow, date. mth, yr)
  //Wire.requestFrom(DS1307_CTRL_ID, 7);
  //for(int i=0; i<7; i++)
  //{
    // store data in raw bcd format
  //  rtc_bcd[i]=Wire.receive();
  //
	//}
	i2c_read_bytes(DS1307_CTRL_ID, rtc_bcd, 7);
}

// update the data on the IC from the bcd formatted data in the buffer
void RTC_Save(void)
{
	uint8_t buf[8], i;
	buf[0] = 0x00;
  //Wire.beginTransmission(DS1307_CTRL_ID);
  //Wire.send(0x00); // reset register pointer
  for(i=0; i<7; i++)
  {
    //Wire.send(rtc_bcd[i]);
		buf[i+1] = rtc_bcd[i];
  }
  //Wire.endTransmission();
	i2c_write_bytes(DS1307_CTRL_ID, buf, 8);
	
}


// PUBLIC FUNCTIONS
void RTC_Get(uint8_t *rtc, uint8_t refresh)   // Aquire data from buffer and convert to int, refresh buffer if required
{
	int i;
  if(refresh) RTC_Read();
  for(i=0;i<7;i++)  // cycle through each component, create array of data
  {
	rtc[i]=RTC_GetIndividual(i, 0);
  }
}

int RTC_GetIndividual(uint8_t c, uint8_t refresh)  // aquire individual RTC item from buffer, return as int, refresh buffer if required
{
	int v=-1;
  if(refresh) RTC_Read();
  switch(c)
  {
  case DS1307_SEC:
    v=(10*((rtc_bcd[DS1307_SEC] & (DS1307_HI_SEC))>>4))+(rtc_bcd[DS1307_SEC] & (DS1307_LO_BCD));
	break;
  case DS1307_MIN:
    v=(10*((rtc_bcd[DS1307_MIN] & (DS1307_HI_MIN))>>4))+(rtc_bcd[DS1307_MIN] & (DS1307_LO_BCD));
	break;
  case DS1307_HR:
    v=(10*((rtc_bcd[DS1307_HR] & (DS1307_HI_HR))>>4))+(rtc_bcd[DS1307_HR] & (DS1307_LO_BCD));
	break;
  case DS1307_DOW:
    v=rtc_bcd[DS1307_DOW] & (DS1307_LO_DOW);
	break;
  case DS1307_DATE:
    v=(10*((rtc_bcd[DS1307_DATE] & (DS1307_HI_DATE))>>4))+(rtc_bcd[DS1307_DATE] & (DS1307_LO_BCD));
	break;
  case DS1307_MTH:
    v=(10*((rtc_bcd[DS1307_MTH] & (DS1307_HI_MTH))>>4))+(rtc_bcd[DS1307_MTH] & (DS1307_LO_BCD));
	break;
  case DS1307_YR:
    v=(10*((rtc_bcd[DS1307_YR] & (DS1307_HI_YR))>>4))+(rtc_bcd[DS1307_YR] & (DS1307_LO_BCD))+(DS1307_BASE_YR);
	break;
  } // end switch
  return v;
}

void RTC_Set(int c, int v)  // Update buffer, then update the chip
{
  switch(c)
  {
  case DS1307_SEC:
    if(v<60 && v>-1)
    {
	//preserve existing clock state (running/stopped)
	int state=rtc_bcd[DS1307_SEC] & DS1307_CLOCKHALT;
	rtc_bcd[DS1307_SEC]=state | ((v / 10)<<4) + (v % 10);
    }
    break;
  case DS1307_MIN:
    if(v<60 && v>-1)
    {
	rtc_bcd[DS1307_MIN]=((v / 10)<<4) + (v % 10);
    }
    break;
  case DS1307_HR:
  // TODO : AM/PM  12HR/24HR
    if(v<24 && v>-1)
    {
	rtc_bcd[DS1307_HR]=((v / 10)<<4) + (v % 10);
    }
    break;
  case DS1307_DOW:
    if(v<8 && v>-1)
    {
	rtc_bcd[DS1307_DOW]=v;
    }
    break;
  case DS1307_DATE:
    if(v<31 && v>-1)
    {
	rtc_bcd[DS1307_DATE]=((v / 10)<<4) + (v % 10);
    }
    break;
  case DS1307_MTH:
    if(v<13 && v>-1)
    {
	rtc_bcd[DS1307_MTH]=((v / 10)<<4) + (v % 10);
    }
    break;
  case DS1307_YR:
    if(v<13 && v>-1)
    {
	rtc_bcd[DS1307_YR]=((v / 10)<<4) + (v % 10);
    }
    break;
  } // end switch
  RTC_Save();
}

void RTC_Stop(void)
{
	// set the ClockHalt bit high to stop the rtc
	// this bit is part of the seconds byte
    rtc_bcd[DS1307_SEC]=rtc_bcd[DS1307_SEC] | DS1307_CLOCKHALT;
    RTC_Save();
}

void RTC_Start(void)
{
	// unset the ClockHalt bit to start the rtc
	// TODO : preserve existing seconds
    rtc_bcd[DS1307_SEC]=rtc_bcd[DS1307_SEC] & (~DS1307_CLOCKHALT);
	RTC_Save();
}


void RTC_SetOutput(int c)  
{
  int out;
	uint8_t buf[2];
  switch(c)
  {
  // case HIGH :
	// out=DS1307_HIGH_BIT;
  // break;
  // case LOW :
	// out=DS1307_LOW_BIT;
  // break;
  case DS1307_SQW1HZ :
	out=DS1307_SQW1HZ_BIT;
  break;
  case DS1307_SQW4KHZ :
	out=DS1307_SQW4KHZ_BIT;
  break;
  case DS1307_SQW8KHZ :
	out=DS1307_SQW8KHZ_BIT;
  break;
  case DS1307_SQW32KHZ :
	out=DS1307_SQW32KHZ_BIT;
  break;
  default: 
	out=DS1307_LOW_BIT;
  }
	buf[0] = 0x07;
	buf[1] = out;
	i2c_write_bytes(DS1307_CTRL_ID, buf, 2);
  //Wire.beginTransmission(DS1307_CTRL_ID);
  //Wire.send(0x07); 
  //Wire.send(out);
  //Wire.endTransmission();
}

int RTC_GetOutput(void)  
{
	uint8_t out;
	int c=-1;
	i2c_write(DS1307_CTRL_ID, 0x07);
  //Wire.beginTransmission(DS1307_CTRL_ID);
  //Wire.send(0x07);
  //Wire.endTransmission();

  //Wire.requestFrom(DS1307_CTRL_ID, 1);
	i2c_read(DS1307_CTRL_ID, &out);
	USART2_PutString("OutBit is:");
	USART2_PutInt(out);
  
  
  switch(out)
  {
  // case DS1307_HIGH_BIT :
	// c=HIGH;
  // break;
  // case DS1307_LOW_BIT :
	// c=LOW;
  // break;
  case DS1307_SQW1HZ_BIT :
	c=DS1307_SQW1HZ;
  break;
  case DS1307_SQW4KHZ_BIT :
	c=DS1307_SQW4KHZ;
  break;
  case DS1307_SQW8KHZ_BIT :
	c=DS1307_SQW8KHZ;
  break;
  case DS1307_SQW32KHZ_BIT :
	c=DS1307_SQW32KHZ;
  break;
  }
  return c;
}

void RTC_SendCommand(uint8_t command)
{	
	//Wire.beginTransmission( _address);
	//Wire.write(command);
	//Wire.endTransmission();
	i2c_write(DS1307_CTRL_ID, command);
}
