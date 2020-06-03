#ifndef Resnik_h
#define Resnik_h


#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <EEPROM.h>
#include "DS3231_Logger.h"
// #include "MCP3421.h"
#include <Adafruit_ADS1015.h> //Include ADC interface
// #include <Adafruit_MCP4725.h> //Include DAC interface
#include <MCP4725.h>  //Include custom DAC library
#include <MCP23018.h>
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

#define VPRIME 1
#define VBETA 2


#define INTERNAL 0
#define EXTERNAL 1

#define MODEL_1v0
#define MODEL_0v0

//Define CBI macro
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif

enum board
{
    Model_0v0 = 0,
    Model_1v0 = 1,
    Model_2v0 = 2
};

enum build
{
	Build_A = 0,
	Build_B = 1,
	Build_C = 2
};

enum temp_val
{
	Therm_Val = 0,
	RTC_Val = 1
};

////////////////////////////PIN DEFINITIONS///////////////////////

/**
 * @class Resnik
 * @brief An Arduino-compatible library for utilizing the basic and logging
 * features of the Resnik data logger
 * @details Data-logger management.
 * Basic operations, power management, on-board sensing,
 * and links to external devices.
 */
class Resnik
{

	public:
    /**
     * @brief
     * Instantiate the Resnik data-logger class
     *
     * @param[in] Model_: Not currently used for Resnik
     *
     * @param[in] Specs_: Defines the number of on-board I2C device addresses.
     *                      **Build_A**: **DEFAULT** 6 on-board I2C devices.
     *                      **Build_B**: 0 on-board I2C devices.
     *
    */
		Resnik(board Model_ = Model_0v0, build Specs_ = Build_A); //Use Build_A by default
    // Resnik();

    /**
     * @brief Begin with a list of attached I2C devices
     *
     * @param[in] *Vals: List of I2C addresses for external sensors
     * @param[in] NumVals: The length of the *Vals list
     * @param[in] Header_: A header string for the data file
     */
		int begin(uint8_t *Vals, uint8_t NumVals, String Header_);
    /**
     * @brief Begin by passing a header string; default empty
     *
     * @param[in] Header_: A header string for the data file
     */
		int begin(String Header_ = "");

    /**
     * Write a string to the log file.
     *
     * @param[in] Val: The string to be written
     */
		int LogStr(String Val);

    /**
     * @brief BSCHULZ1701: WHAT EXACTLY DOES THIS FUNCTION DO?
     *
     * @param[in] LineIndex: The desired line number
     * @param[in] DataIndex: The desired byte at which to start in the file
     */
		String ReadStr(uint8_t LineIndex, uint32_t DataIndex);

    /**
     * @brief Set the color of the LED
     *
     * @param[in] Val: 4-byte (R, G, B, alpha) color
     */
		void LED_Color(unsigned long Val);

    /**
     * @brief Main function that loops infinititely during runtime
     * BSCHULZ1701: I changed *f to *Update to match the CPP. Hope that does
     * not break anything!
     *
     * @param[in] *Update: Pointer to a function in the Arduino sketch (ino);
     *                     this retrieves new sensor data as a String object
     * @param[in] LogInterval: Number of seconds between log events
     */
		void Run(String (*Update)(void), unsigned long LogInterval);

    /**
     * @brief Read voltage from the external 16-bit ADC
     * @param[in] Pin (range 0-3) -- which pin to read?
     */
		float GetVoltage(uint8_t Pin); //Read ADC

    /**
     * @brief Set voltage on the 12-bit digital-to-analog converter
     * @param[in] Val: 0 to 4095, which scales from 0 to Vbus
     * @param[in] Gain: WHAT? WHY BOOL?
     *
     * BSCHULZ1701: WHY IS THE GAIN A BOOLEAN? AND IS THIS FROM 0 TO VBUS?
     * AND SHOULD THIS BE A PRIVATE UTILITY FUNCTION?
     */
		uint8_t SetVoltageRaw(uint16_t Val, bool Gain = GAIN_1X); //Set DAC with raw input, default to 1x gain

    /**
     * @brief Set voltage on the 12-bit digital-to-analog converter
     * @param[in] Val: Desired voltage value, in volts.
     */
		uint8_t SetVoltage(float Val); //Set DAC to nearest interpolated value

    /**
     * @brief Writes date/time, on-board sensors, and external string to SD
     * @param *Update: External update function from Arduino sketch
     */
		void AddDataPoint(String (*Update)(void));

    /**
     * @brief Obtain date/time, P/T/RH, temperatures, and voltages from board
     */
		String GetOnBoardVals();

    /**
     * @brief Create a new log file, following serial numbering
     */
		void InitLogFile();

		// void GetEnviro(); //Get/update enviromental values from BME280 and RTC
		// float GetBatVoltage();
		// float GetBatPer();
		// void GetPowerStats(); //Get all voltage and current values, converter/solar states, etc //ADD!!!!!!!!

		// void ResetWD();
		// void PowerOB(bool State);

    /**
     * @brief Determine which input has power and set up power path from that
     */
		uint8_t PowerAuto();

    /**
     * @brief Power from Main, backup, or off
     * BSCHULZ1701: I am not sure what this does!
     * @param[in] State:
     */
		void PowerAux(uint8_t State);

		// void PowerFeather(bool State); //Turn feather power on or off  //ADD!!!!!

    /**
     * @brief Set the I2C state
     * @param State: True or False gives On or Off
     */
		void I2CState(bool State); //Use on board of external I2C

    //////////////////////////////
		// Pin definitions - Public //
    //////////////////////////////

    /// SD card chip select **Pin 4**
		int SD_CS = 4;

    /// Pin controlling on/off state of RGB LED **Pin 20**
		uint8_t BuiltInLED = 20;

    /// Pin controlling red intensity of RGB LED **Pin 13**
		uint8_t RedLED = 13;

    /// Pin controlling green intensity of RGB LED **Pin 15**
		uint8_t GreenLED = 15;

    /// Pin controlling blue intensity of RGB LED **Pin 14**
		uint8_t BlueLED = 14;

		// uint8_t VRef_Pin = 2;
		// uint8_t ThermSense_Pin = 1;
		// uint8_t BatSense_Pin = 0;

		// uint8_t VSwitch_Pin = 3;

    /// Chip-detect pin for the SD card **Pin 1**
		uint8_t SD_CD = 1;

		// uint8_t Ext3v3Ctrl = 19;

    /// Switch to enable/disable I2C **Pin 21**
		uint8_t I2C_SW = 21;

		// uint8_t PG = 18;
		// uint8_t ExtInt = 11;

    /// Interrupt from RTC **Pin 10** ***DIFFERENT IN CPP!!!!***
		uint8_t RTCInt = 10;
    /// Interrupt from Log button **Pin 2** ***DIFFERENT IN CPP!!!!***
		uint8_t LogInt = 2;

		// uint8_t WDHold = 23; //ADD TO DOCUMENTATION!
		// uint8_t BatSwitch = 22; //ADD TO DOCUMENTATION!

    /// USART Transmit
		uint8_t TX = 11; //ADD TO DOCUMENTATION!

    /// USART Receive
		uint8_t RX = 10; //ADD TO DOCUMENTATION!

		// uint8_t D0 = 3; //ADD TO DOCUMENTATION!

    /// BSCHULZ1701: WHAT ARE THESE??
		uint8_t C0 = 18;
    /// BSCHULZ1701: WHAT ARE THESE??
		uint8_t C1 = 19;

    /// Primary bus switch pin (BSCHULZ1701: is this in addition to the physical switch?)
		uint8_t Sw_Bus_Prime = 23;
    /// Secondary bus switch pin (BSCHULZ1701: is this in addition to the physical switch?)
		uint8_t Sw_Bus_Sec = 22;

    /// IO Exp PORT B (BSCHULZ1701: what is this in plain English / purpose?)
		uint8_t PG_3v3_Core = 1;
    /// IO Exp PORT B (BSCHULZ1701: turn Feather on if True, I guess?)
		uint8_t FeatherEN = 7;

    /// GPIO pin D0 **Arduino Pin 12**
		uint8_t D0 = 12;
    /// GPIO pin D1 **Arduino Pin 25**
		uint8_t D1 = 25;
    /// GPIO pin D2 **Arduino Pin 3**
		uint8_t D2 = 3;
    /// GPIO pin D3 **Arduino Pin 26**
		uint8_t D3 = 26;

    /// Switch the Analog-Digital Converter on (true) or off (false) **Pin 0**
		uint8_t ADC_Sense_SW = 0;

    /// Feather pin: should this be public or private?
		uint8_t FeatherRTS = 31;
    /// Feather pin: should this be public or private?
		uint8_t FeatherCTS = 30;
    /// Feather pin: should this be public or private?
		uint8_t FeatherGPIO = 29;
    /// Feather pin: should this be public or private?
		uint8_t CS_Ext = 24;

    /// WHAT IS THIS?
		uint8_t GlobalInt = 28;



    /// Resnik data logger library version
		const String LibVersion = "0.0.0";

	protected:
		// float TempConvert(float V, float Vcc, float R, float A, float B, float C, float D, float R25);
		void Blink();
		// void StartLog();
		// void Log();
		void virtual Log();
		void virtual ButtonLog();
		static void isr0();
		static void isr1();
		static Resnik* selfPointer;

		static void DateTimeSD(uint16_t* date, uint16_t* time);
		void DateTimeSD_Glob(uint16_t* date, uint16_t* time);
		void sleepNow();
		void turnOffSDcard();
		void turnOnSDcard();
		void GetTime();
		void I2CTest();
		void SDTest();
		void ClockTest();
		// void BatTest();
		// void PowerTest();
		void EnviroStats();
		int freeMemory(); //DEBUG!

		DS3231_Logger RTC;
		// MCP3421 adc;
		BME EnviroSense;
		MCP4725 DAC; //Instatiate DAC
		Adafruit_ADS1115 ADC_OB; //Initialize on board (power moitoring) ADC
		Adafruit_ADS1115 ADC_Ext;  //Initialize external (sensor) ADC
		MCP23018 IO;
		// float A = 0.003354016;
		// float B = 0.0003074038;
		// float C = 1.019153E-05;
		// float D = 9.093712E-07;
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
		uint8_t NumADR_OB = 6;
		uint8_t I2C_ADR_OB[6] = {0x68, 0x20, 0x48, 0x49, 0x62, 0x77}; //Clock, IO Expander, ADC_OB, ADC_Ext, DAC, BME

		// float BatteryDivider = 2.0; //Default for v1.0

		// float Temp_BME = 0; //FIX! Make non-global, pass array??
		// float Temp_RTC = 0;
		// float RH = 0;
		// float Pres = 0;

		board Model;
		build Specs;

		volatile bool LogEvent = false; //Used to test if logging should begin yet
		volatile bool NewLog = false; //Used to tell system to start a new log
		// volatile bool ManualLog = false; //Used to add point to log by pressing the log button
		volatile int AwakeCount = 0;

		char FileNameC[11]; //Used for file handling
		char FileNameTestC[11]; //Used for file handling
		bool SD_Init = false;
		SdFat SD;
		byte  keep_SPCR;
		byte keep_ADCSRA;

		uint16_t LogCountPush = 5; //Number of logs to take before sending data off
		uint16_t LogCount = 0; //Number of logs since last data write
		uint16_t Index = 0; //Index of data entry USE???? FIX!
		uint32_t SDIndex = 0; //Index of data point in SD card file
		uint32_t LastSDIndex = 0; //Index as last data dump
};

#endif
