/*
Margay Library
Licensed: GNU GPL v3

Written by:
Bobby Schulz
Andy Wickert
*/


#ifndef MARGAY_h
#define MARGAY_h

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <EEPROM.h>
#include "DS3231_Logger.h"
#include "MCP3421.h"
#include "SdFat.h"
#include "BME.h"


#define RED 0xFFFF0000L
#define GREEN 0xFF00FF00L
#define BLUE 0xFF0000FFL
#define MAROON 0xFF800000L
#define GOLD 0xFFFFD700L
#define ORANGE 0xFFFFA500L
#define PURPLE 0xFF800080L
#define CYAN 0xFF00FFFF
#define BLACK_ALERT 0x802019FF

#define ON 1
#define OFF 0

//Define CBI macro
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif

enum board
{
    MODEL_0v0 = 0,
    MODEL_1v0 = 1,
    MODEL_2v0 = 2,
    MODEL_2v1 = 2,
    MODEL_2v2 = 2
};

enum build
{
	BUILD_A = 0,
	BUILD_B = 1,
	BUILD_C = 2,
	BUILD_D = 3
};

enum temp_val
{
	therm_val = 0,
	RTC_val = 1
};

////////////////////////////PIN DEFINITIONS///////////////////////

class Margay
{

	public:
		Margay(board model_ = MODEL_2v2, build specs_ = BUILD_B);
    int begin(uint8_t *Vals, uint8_t numVals, String header_);
		int begin(String header_ = "");

		int logStr(String val);
		void LED_Color(unsigned long val);
		void run(String (*f)(void), unsigned long logInterval);
		float getVoltage();
		void addDataPoint(String (*update)(void));
		void initLogFile();

		void setExtInt(uint8_t n, String header_entry = "nInterrupts,");
		uint16_t getExtIntCount(bool reset0 = true);
		void resetExtIntCount(uint16_t start = 0);

		float getTemp(temp_val val = therm_val);
		float getBatVoltage();
		float getBatPer();

		void initADC(uint8_t desiredResolution);
		void resetWDT();
		void powerOB(bool State);
		void powerAux(bool State);

		//Pin definitions
		int SD_CS = 4;
		uint8_t BuiltInLED = 20;
		uint8_t RedLED = 13;
		uint8_t GreenLED = 15;
		uint8_t BlueLED = 14;

		uint8_t VRef_Pin = 2;
		uint8_t ThermSense_Pin = 1;
		uint8_t BatSense_Pin = 0;

		uint8_t VSwitch_Pin = 3;
		uint8_t SD_CD = 1;

		uint8_t Ext3v3Ctrl = 19;
		uint8_t I2C_SW = 21;
		uint8_t PG = 18;
		uint8_t ExtInt = 11;
		uint8_t RTCInt = 10;
		uint8_t LogInt = 2;
		uint8_t WDHold = 23; //ADD TO DOCUMENTATION!
		uint8_t BatSwitch = 22; //ADD TO DOCUMENTATION!
		uint8_t TX = 11; //ADD TO DOCUMENTATION!
		uint8_t RX = 10; //ADD TO DOCUMENTATION!
		uint8_t D0 = 3; //ADD TO DOCUMENTATION!

		const String LibVersion = "0.3.0";

	protected:
		float tempConvert(float V, float Vcc, float R, float A, float B, float C, float D, float R25);
		void blinkGood();
		void virtual writeDataToSD();
		void virtual buttonLog();
		static void isr0();
		static void isr1();
		static void isr2();
		static Margay* selfPointer;
    static void dateTimeSD(uint16_t* date, uint16_t* time);
		void dateTimeSD_Glob(uint16_t* date, uint16_t* time);
	    void switchExternalI2C(bool desiredState);
		void sleepNow();
		void turnOffSDcard();
		void turnOnSDcard();
		void getTime();
		String getOnBoardVals();
		void I2Ctest();
		void SDtest();
		void clockTest();
		void batTest();
		void powerTest();
		void enviroStats();
		void extIntCounter();
		int freeMemory(); //DEBUG!
		void _addDataPoint(String data);
		void farmGateI2C(bool initialStateExternalI2C);

		DS3231_Logger RTC;
		MCP3421 adc;
		BME EnviroSense;

		float A = 0.003354016;
		float B = 0.0003074038;
		float C = 1.019153E-05;
		float D = 9.093712E-07;
		String LogTimeDate = "2063/04/05 20:00:00";
		// float Temp[5] = {0}; //Temp Downhole, Temp on board, Temp RTC, Temp Baro
		// float Pressure[2] = {0}; //Downhole pressure, Atmospheric pressure (if applicable)
		bool OBError = false;
		bool SensorError = false;
		bool TimeError = false;
		bool SDError = false; //USE??
		bool BatError = false;
		bool BatWarning = false;
		float BatVoltageError = 3.3; //Low battery alert will trigger if voltage drops below this value
		float BatPercentageWarning = 50; //Percentage at which a warning will be indicated
		String Header = "";
		const char HexMap[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'}; //used for conversion to HEX of string
		char SN[20] = {0}; //Used to store device serial number, 19 chars + null terminator
		// String SN = "FFFF-FFFF-FFFF-FFFF";
		uint8_t NumADR = 0;
		uint8_t I2C_ADR[16] = {0}; //Change length??
		uint8_t NumADR_OB = 1;
		uint8_t I2C_ADR_OB[1] = {0x68}; //ADC, Clock

		float BatteryDivider = 2.0; //Default for v1.0

		board Model;
		build Specs;

		volatile bool LogEvent = false; //Used to test if logging should begin yet
		volatile bool NewLog = false; //Used to tell system to start a new log
		volatile int AwakeCount = 0;

		char FileNameC[11]; //Used for file handling
		char FileNameTestC[11]; //Used for file handling
		bool SD_Init = false;
		bool externalI2COn = false;
		SdFat SD;
		byte  keep_SPCR;
		byte keep_ADCSRA;
};

#endif
