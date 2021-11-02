/******************************************************************************
MonarchSW.h
Library for interfacing with the Project Monarch short wave pyranometer 
Bobby Schulz @ Northern Widget LLC
7/5/2018
https://github.com/NorthernWidget-Skunkworks/Project-Monarch

This script is used to interface to the Monarch shortwave pyranometer module and provide 
conversions for some of the values into the appropriate human-readable units.

"The laws of nature are constructed in such a way as to make the universe as interesting as possible."
-Freeman Dyson

Distributed as-is; no warranty is given.
******************************************************************************/

#ifndef MonarchSW_h
#define MonarchSW_h

#include <Arduino.h>
#include "Wire.h"
#include "math.h"

#define UP 0
#define DOWN 1

#define UVA_ADR 0x02
#define UVB_ADR 0x06
#define ALS_ADR 0x0B
#define WHITE_ADR 0x0D
#define LUXMUL_ADR 0x10
#define IR_MID_ADR 0x13
#define IR_SHORT_ADR 0x15
#define THERM_ADR 0x17

#define XAXIS 0x32
#define YAXIS 0x34
#define ZAXIS 0x36

class MonarchSW 
{
	public:
		MonarchSW(uint8_t Orientation_ = 0); //By default use upward facing 
		uint8_t begin(); 
		uint8_t InitAccel();
		float GetG(uint8_t Axis);
		float GetAngle(uint8_t Axis);
		long GetUVA();
		long GetUVB();
		unsigned int GetALS();
		unsigned int GetWhite();
		float GetLux();
		float GetIR_Short();
		float GetIR_Mid();
		float GetTemp();
		String GetString();
		String GetHeader();

	private:
		const float LuxRes = 0.0036; //Min resolution of lux measurement

		const float A = 0.003354016;
		const float B = 0.0003074038;
		const float C = 1.019153E-05;
		const float D = 9.093712E-07;

		uint8_t ADR = 0x40; //Default address
		uint8_t Accel_ADR = 0x1D; //Default address
		uint8_t Orientation = 0; //Global orientation value 
		float TempConvert(float V, float Vcc, float R, float A, float B, float C, float D, float R25);
		void PrintAllRegs();
		uint8_t WriteByte(uint8_t Adr, uint8_t Pos, uint8_t Val);
		uint8_t ReadByte(uint8_t Adr, uint8_t Pos);
		unsigned int ReadWord(uint8_t Adr, uint8_t Pos);
};

#endif
