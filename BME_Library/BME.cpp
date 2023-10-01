// #include <Adafruit_BME280.h>
#include "BME.h"
#include "Adafruit_BME280.h"
// #include <Adafruit_Sensor.h>

BME::BME()
{
}

uint8_t BME::begin(uint8_t ADR_)
{
	ADR = ADR_;
	Sensor.begin(ADR);
}

float BME::getPressure() //Get pressure in mBar 
{
	return Sensor.readPressure()*0.01; //Convert to mBar
}

float BME::getHumidity()  //Return humidity in % (realtive)
{
	return Sensor.readHumidity();
}

float BME::getTemperature()  //Return temp in C
{
	return Sensor.readTemperature();
}

String BME::getHeader()
{
	return "Pressure Atmos [mBar], Humidity [%], Temp Atmos [C],";
}

String BME::getString()
{
	return String(GetPressure()) + "," + String(GetHumidity()) + "," + String(GetTemperature()) + ",";
}

/////////////////////////////////////////////
// PascalCase: FOR BACKWARDS COMPATIBILITY //
/////////////////////////////////////////////

// This case should be used for classes and objects

float BME::GetPressure() //Get pressure in mBar 
{
	return Sensor.readPressure()*0.01; //Convert to mBar
}

float BME::GetHumidity()  //Return humidity in % (realtive)
{
	return Sensor.readHumidity();
}

float BME::GetTemperature()  //Return temp in C
{
	return Sensor.readTemperature();
}

String BME::GetHeader()
{
	return "Pressure Atmos [mBar], Humidity [%], Temp Atmos [C],";
}

String BME::GetString()
{
	return String(GetPressure()) + "," + String(GetHumidity()) + "," + String(GetTemperature()) + ",";
}

