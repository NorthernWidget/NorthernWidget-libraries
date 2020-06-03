/******************************************************************************
VEML6075.cpp
Library for VEML6075 UV sensor
Bobby Schulz @ Northern Widget LLC
5/16/2018
https://github.com/NorthernWidget/VEML6075

The VEML6075 is an integrated UVA and UVB sensor make by Vishay, with an I2C interface. This
device has independent UVA and UVB channels and a manually variable integration time. 

"There has to be an invisible sun
It gives its heat to everyone
There has to be an invisible sun
That gives us hope when the whole day's done"
-Sting

Distributed as-is; no warranty is given.
******************************************************************************/

#include <Wire.h> // Wire library is used for I2C
// #include <cmath.h>
#include "VEML6075.h"

VEML6075::VEML6075()
// Base library type I2C
{
}

uint8_t VEML6075::begin(uint8_t ADR_) //Initalize with alternate address
{ 
	ADR = ADR_;  //Set non-standard address
	begin(); //Call conventional startup
}

uint8_t VEML6075::begin()
// Initialize library for subsequent pressure measurements
{

    Wire.begin();  // Arduino Wire library initializer

    Wire.beginTransmission(ADR);
    Wire.write(0x00);
    Wire.write(0x00);
    Wire.write(0x00);
    return Wire.endTransmission(true); //Return sucess or failue of I2C connection
}

uint8_t VEML6075::SetIntTime(unsigned int Time)
{
	Config = ReadByte(CONF_CMD, 0); //Update global config value

	//Test all conditions operate as expected!
	if(Time <= 0x04) {  //If defined names are being used
		return WriteConfig((Config | 0x70) & Time);
	}

	else if(Time <= 800 && Time >= 50) {  //If using raw numbers in range
		uint8_t IntMask = ((int)(log(Time % 50)/log(2)) << 4) | 0x0F; //Calculate integration bit mask based on input value
		return WriteConfig((Config | 0x70) & IntMask);
	}

	else if(Time > 800) {  //If greater than max, set to max time
		return WriteConfig((Config | 0x70) & IT800); 
	}

	else if(Time < 50) {  //If less than min, set to min time 
		return WriteConfig((Config | 0x70) & IT50); 
	}
}

uint8_t VEML6075::Shutdown() {  //Places device in shutdown low power mode
	Config = ReadByte(CONF_CMD, 0); //Update global config value
	return WriteConfig(Config | 0x01); //Set shutdown bit
}

uint8_t VEML6075::PowerOn() {  //Turns device on from shutdown mode
	Config = ReadByte(CONF_CMD, 0); //Update global config value
	return WriteConfig(Config & 0xFE); //Clear shutdown bit
}

uint8_t VEML6075::Mode(uint8_t OperatingMode)
{
	Config = ReadByte(CONF_CMD, 0);  //Update global config value
	if(OperatingMode) {
		uint8_t ModeMask = 0xFD;
		return WriteConfig(Config & ModeMask);  //Clear UV_AF bit
	}

	else {
		uint8_t ModeMask = 0x02;
		return WriteConfig(Config | ModeMask); //Set UV_AF bit
	}
}

uint8_t VEML6075::StartConversion()
{
	Config = ReadByte(CONF_CMD, 0);
	if((Config | 0x02) >> 1) {  //Check if single shot mode is active
		Config = Config | 0x04; //Set UV_TRIG
		StartTime = millis();  //Set global start time
		return WriteConfig(Config); //Set value
	}

	else {
		Mode(SINGLE_SHOT); //Set single shot mode
		Config = Config | 0x04; //Set UV_TRIG
		StartTime = millis(); //Set global start time
		return WriteConfig(Config); //Set value
	}
}

float VEML6075::GetUVA() 
{
	Config = ReadByte(CONF_CMD, 0);
	long ConversionTime = (1 << ((Config & 0x70) >> 4))*50; //Calculate ms conversion time = (2^UV_IT) * 50
	if((Config | 0x02) >> 1) { //Test single shot bit
		while((millis() - StartTime) < ConversionTime);  //Wait if more time is needed
	}
	//In either case, measurment process is the same 
	float UVA = ReadWord(UVA_CMD);
	float Comp1 = ReadWord(COMP1_CMD);
	float Comp2 = ReadWord(COMP2_CMD);
	float UVA_Comp = UVA - a*Comp1 - b*Comp2;

	return UVA_Comp;
}

float VEML6075::GetUVB() 
{
	Config = ReadByte(CONF_CMD, 0);
	long ConversionTime = (1 << ((Config & 0x70) >> 4))*50; //Calculate ms conversion time = (2^UV_IT) * 50
	if((Config | 0x02) >> 1) { //Test single shot bit
		while((millis() - StartTime) < ConversionTime);  //Wait if more time is needed
	}
	//In either case, measurment process is the same 
	float UVB = ReadWord(UVB_CMD);
	float Comp1 = ReadWord(COMP1_CMD);
	float Comp2 = ReadWord(COMP2_CMD);
	float UVB_Comp = UVB - c*Comp1 - d*Comp2;
	
	return UVB_Comp;
}


uint8_t VEML6075::SendCommand(uint8_t Command)
{
    Wire.beginTransmission(ADR);
    Wire.write(Command);
    return Wire.endTransmission(false);
}

uint8_t VEML6075::WriteConfig(uint8_t NewConfig)
{
	Wire.beginTransmission(ADR);
	Wire.write(CONF_CMD);  //Write command code to Config register
	Wire.write(NewConfig);
	uint8_t Error = Wire.endTransmission();

	if(Error == 0) {
		Config = NewConfig; //Set global config if write was sucessful 
		return 0;
	}
	else return Error; //If write failed, return failure condition
}

int VEML6075::ReadByte(uint8_t Command, uint8_t Pos) //Send command value, and high/low byte to read, returns desired byte
{
	uint8_t Error = SendCommand(Command);
	Wire.requestFrom(ADR, 2, true);
	uint8_t ValLow = Wire.read();
	uint8_t ValHigh = Wire.read();
	if(Error == 0) {
		if(Pos == 0) return ValLow;
		if(Pos == 1) return ValHigh;
	}
	else return -1; //Return error if read failed

}

int VEML6075::ReadWord(uint8_t Command)  //Send command value, returns entire 16 bit word
{
	uint8_t Error = SendCommand(Command);
	Wire.requestFrom(ADR, 2, true);
	uint8_t ByteLow = Wire.read();  //Read in high and low bytes (big endian)
	uint8_t ByteHigh = Wire.read();
	if(Error == 0) return ((ByteHigh << 8) | ByteLow); //If read succeeded, return concatonated value
	else return -1; //Return error if read failed
}