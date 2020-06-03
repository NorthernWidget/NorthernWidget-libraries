
#ifndef BME_h
#define BME_h

#include "Arduino.h"
#include "Adafruit_BME280.h"
// #include <Adafruit_Sensor.h>

class BME
{
	public:
		BME();
		uint8_t begin(uint8_t ADR_ = 0x77); //use default address
		float GetPressure();
		float GetHumidity();
		float GetTemperature();
		String GetString();
		String GetHeader();
	private:
		Adafruit_BME280 Sensor;
		uint8_t ADR = 0x77; //Default global sensor address 
};

#endif