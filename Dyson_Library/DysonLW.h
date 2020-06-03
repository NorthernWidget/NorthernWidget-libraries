/******************************************************************************
DysonLW.h
Library for interfacing with the Project Dyson long wave pyranometer 
Bobby Schulz @ Northern Widget LLC
7/5/2018
https://github.com/NorthernWidget-Skunkworks/Project-Dyson

This script is used to interface to the Dyson shortwave pyranometer module and provide 
conversions for some of the values into the appropriate human-readable units.

"The laws of nature are constructed in such a way as to make the universe as interesting as possible."
-Freeman Dyson

Distributed as-is; no warranty is given.
******************************************************************************/

#ifndef DYSONLW_h
#define DYSONLW_h

#include "Arduino.h"

#define ADC_ADR 0x4A

#define ADC_CONF 0x01
#define ADC_CONV 0x00
#define ADC_CONF_MASK 0xC383
// #define ADC0 0xC383
// #define ADC1 0xD383
// #define ADC2 0x6383
// #define ADC3 0x7200

#define ADC0 0x00
#define ADC1 0x01
#define ADC2 0x02
#define ADC3 0x03

class DysonLW
{
	public:
		DysonLW();
		uint8_t begin(); //use default address
		float GetThermo();
		float GetThermoC(uint8_t Accuracy = 0); //Low accuracy by default 
		float GetTemp();
		String GetHeader();
		String GetString();

	private:
		//FIX Constants for different thermistor!
		// const float A = 0.003354016;
		// const float B = 0.0003074038;
		// const float C = 1.019153E-05;
		// const float D = 9.093712E-07;
		const float Beta = 3960; //Beta value for thermistor, MAKE NOT FLOAT??

		float TempConvert(float V, float Vcc, float R, float A, float B, float C, float D, float R25);
		float TempConvert(float V, float Vcc, float R, float Beta, float R25);
		unsigned int GetADC(unsigned int Num);
		uint8_t WriteWord_LE(uint8_t Adr, uint8_t Command, unsigned int Data);
		int ReadWord_LE(uint8_t Adr, uint8_t Command);
		uint8_t SendCommand(uint8_t Adr, uint8_t Command);
};

#endif
