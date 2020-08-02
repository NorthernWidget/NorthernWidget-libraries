/******************************************************************************
DS3231_Logger.h
A simplified library for the DS3231, focused on data logger applications
Bobby Schulz @ Northern Widget LLC
4/4/2018

The DS3231 is a high accuracy tempurature compensated RTC. This chip allows for time to be accurately kept
over long periods of time, and waking up a logger device when required to take measurments or tend to sensors

"That's not fair. That's not fair at all. There was time now. There was, was all the time I needed..."
-Henery Bemis

Distributed as-is; no warranty is given.
******************************************************************************/

#ifndef DS3231_LOGGER_h
#define DS3231_LOGGER_h

#include "Arduino.h"
#include <Wire.h>

#define SECOND 5
#define MINUTE 4
#define HOUR 3
#define DAY 2
#define MONTH 1
#define YEAR 0

class DS3231_Logger
{
	public:
		DS3231_Logger();
		int begin(void);
		int setTime(int Year, int Month, int Day, int Hour, int Min, int Sec);
		String getTime(int mode);
		float getTemp();
		int getValue(int n);
		int setAlarm(unsigned int Seconds);
		int clearAlarm();

		//int GetTime();
		//String ToString();
	private:
		int ADR = 0x68; //Address of DS3231 (non-variable)
		int Time_Date[6]; //Store date time values of integers 

};

#endif
