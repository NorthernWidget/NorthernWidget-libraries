/******************************************************************************
VEML6030.cpp
Library for VEML6030 Ambient light sensor
Bobby Schulz @ Northern Widget LLC
5/16/2018
https://github.com/NorthernWidget/VEML6030

The VEML6030 is an integrated ambient light sensor made by Vishay, with an I2C interface. 
This device has a manually variable gain and integration time. This library was made to allow for
low level interfacing to the device as well as high level functions like auto-ranging.

"Some people seemed to get all sunshine, and some all shadow…"
-Louisa May Alcott

Distributed as-is; no warranty is given.
******************************************************************************/

#include <Wire.h> // Wire library is used for I2C
// #include <cmath.h>
#include "VEML6030.h"

VEML6030::VEML6030()
// Base library type I2C
{
}


uint8_t VEML6030::begin(uint8_t _ADR) // Initialize library for subsequent pressure measurements
{
	ADR = _ADR;
    Wire.begin();  // Arduino Wire library initializer

    Wire.beginTransmission(ADR);
    Wire.write(0x00);
    Wire.write(0x00);
    Wire.write(0x00);
    return Wire.endTransmission(true); //Return sucess or failue of I2C connection
}

uint8_t VEML6030::SetIntTime(unsigned int TimeBits)
{
	Config = ReadWord(CONF_CMD); //Update global config value
	return WriteConfig((Config & 0xFC3F) | TimeBits);
}

unsigned int VEML6030::GetIntTime()
{
	Config = ReadWord(CONF_CMD); //Update global config value
	int X2X1 = (Config >> 6) & 0x03; 
	int X3 = ReadBit(Config, 8);
	int X4 = ReadBit(Config, 9);
	IntTime = 100*pow(2, X2X1)/pow(2, X3 + X4);  //Do some ugly math to go from bit pattern to value
	return IntTime; 
}

unsigned int VEML6030::IntTime2Bits(unsigned int Time)
{
	uint8_t X2X1 = (int)(log(Time/100)/log(2));
	uint8_t X4X3 = (int)(((Time % 100) / 25) + 2*(Time % 50)/25);
	return ((X4X3 << 2) | (X2X1)) << 6; 
}

uint8_t VEML6030::SetGain(unsigned int GainVal)
{
	Config = ReadWord(CONF_CMD); //Update global config value
	// Serial.print("Config = "); //DEBUG!
	// Serial.print(Config); //DEBUG! 
	// Serial.print(" "); //DEBUG!
	// Serial.println((Config & 0xE7FF) | GainVal); //DEBUG!
	return WriteConfig((Config & 0xE7FF) | GainVal);
}

float VEML6030::GetGain()
{
	Config = ReadWord(CONF_CMD); //Update global config value
	// Serial.println(Config); //DEBUG!
	int X0 = ReadBit(Config, 11);
	int X1 = ReadBit(Config, 12);
	Gain = pow(2, X0 - 3*X1);
	return Gain; 
}

float VEML6030::GetGain(uint8_t GainBits)
{
	uint8_t X0 = ReadBit(GainBits, 1);
	uint8_t X1 = ReadBit(GainBits, 0);
	Gain = pow(2, X0 - 3*X1);
	return Gain; 
}

unsigned int VEML6030::Gain2Bits(float GainVal)
{
	for(int i = 0; i < 4; i++) {  //Use linear search to avoid float math and increase speed
		if(GainVals[i] == GainVal) {
			return (i << 11); //if entries match, return bits
		}
	}
	return 0x1000; //Return gain of 1/8 if not a valid gain value 
}

uint8_t VEML6030::Shutdown() //Places device in shutdown low power mode
{  
	Config = ReadWord(CONF_CMD); //Update global config value
	return WriteConfig(Config | 0x01); //Set shutdown bit
}

uint8_t VEML6030::PowerOn() //Turns device on from shutdown mode
{  
	Config = ReadWord(CONF_CMD); //Update global config value
	return WriteConfig(Config & 0xFFFE); //Clear shutdown bit
}

uint8_t VEML6030::IntOn() //Turns interrupt on
{  
	Config = ReadWord(CONF_CMD); //Update global config value
	return WriteConfig(Config | 0x02); //Set Int bit
}

uint8_t VEML6030::IntOff() //Turns interrupt off
{  
	Config = ReadWord(CONF_CMD); //Update global config value
	return WriteConfig(Config & 0xFFFD); //Set Int bit
}

uint8_t VEML6030::SetPersistence(uint8_t Persistance)  //Sets number of consectuive values for interrupt
{  
	Config = ReadWord(CONF_CMD); //Update global config value
	return WriteConfig((Config | 0x30) & Persistance); //Set Persistance bits
}
//CHECK/FIX potential rounding issues!
uint8_t VEML6030::AutoRange()  //Automatically finds maximum gain and resolution values for given irradiance
{
	// PowerSaveOff(); //Turn power save off for fastest reading
	SetIntTime(IT25); //Set to minimum integration time
	SetGain(GAIN_1_8); //Set to minmum gain
	// Serial.print("UnRanged Result ="); Serial.println(ReadWord(CONF_CMD), HEX); //DEBUG!
	PowerOn();
	// long StartTime = millis(); //DEBUG! 
	// while(millis() - StartTime < 50) {  //DEBUG!
	// 	Serial.println(ReadWord(INT_CMD), HEX); //DEBUG!
	// }  //DEBUG!
	delay(30); //Wait for new sample
	float TestLux = GetLux(); //Get new lux value
	unsigned long HighLux = 120796;  //Start at max value
	unsigned int NewIntTime = 25; //Default to min value
	float NewGainHigh = 0.125; //Default to min value
	float NewGainLow = 0.25; //Default to 2nd lowest value
	float NewGain = 0.125; //Default to min value
	// Serial.print("LuxTest = "); //DEBUG!
	// Serial.println(TestLux); //DEBUG!
	//Increment through lux ranges to find desired gain range
	if(TestLux < 236) {  //If Lux is too small to measure at max values (<1.8432) or in minimum range, simply set to highest gain and integration time
		NewIntTime = 800;
		NewGain = 2;
	}
	else {  //If lux is not outside low range, search for a value
		for(int i = 0; i < 6; i++) {
			if(TestLux < HighLux && TestLux >= HighLux/2.0) {
				NewIntTime = NewIntTime * ceil(pow(2, i));
				break; //breakout of for loop since result is found
			}
			else HighLux = ceil(HighLux/2.0); //If not found, go to next lux range
		}

		if(TestLux < HighLux * 0.0625) NewGain = 2; //If below the lowest max for integration range, set to max gain
		else {  //Otherwise search for a new value 
			for(int g = 1; g < 3; g++) {
				if(TestLux < HighLux * (0.125/NewGainHigh) && TestLux >= HighLux * (0.125/NewGainLow)) {
					NewGain = NewGainHigh;
					break; //Break loop once gain is found
				}
				else {
					NewGainHigh = NewGainLow;
					NewGainLow = GetGain(GainValBits[g + 1]);
				}
			}
		}
	}

	// Serial.print("AutoRange Vals = "); Serial.print(NewGain); Serial.print(" "); Serial.println(NewIntTime); //DEBUG!
	unsigned int GainBits = Gain2Bits(NewGain); //Convert new gain value
	unsigned int IntBits = IntTime2Bits(NewIntTime); //Convert to new integration time
	// Serial.print("Bits = "); Serial.print(GainBits, HEX); Serial.print(" "); Serial.println(IntBits, HEX); //DEBUG!

	SetGain(GainBits);
	SetIntTime(IntBits);
	// Serial.print("AutoRanged Result ="); Serial.println(ReadWord(CONF_CMD), HEX); //DEBUG!
}

float VEML6030::GetLux() 
{	//Add non-linear correction! 
	GetGain(); //Update global values
	GetIntTime(); 
	float Resolution = (1.8432/((float)IntTime/25.0))*(0.125/Gain);
	// Serial.print("TEST = "); //DEBUG!
	// Serial.print(Resolution); //DEBUG!
	// Serial.print(" "); //DEBUG!
	// Serial.print(IntTime); //DEBUG! 
	// Serial.print(" "); //DEBUG!
	// Serial.println(Gain); //DEBUG!
	return GetALS()*Resolution; //Return scaled Lux mesurment
}

unsigned int VEML6030::GetALS() 
{
	return ReadWord(ALS_CMD);
}

unsigned int VEML6030::GetWhite() 
{
	return ReadWord(WHITE_CMD);
}

int VEML6030::GetIntStatus() //Will return -1 for low threshold exceed, +1 for high threshold, and 0 if no interrupt has occored
{ 
	unsigned int IntStatus = ReadWord(INT_CMD);
	// uint8_t ThLow = ReadBit(IntStatus, 15); //Read low threshold bit  //REMOVE!
	// uint8_t ThHigh = ReadBit(IntStatus, 14);  //Read high threshold bit  //REMOVE!
	return -1*ReadBit(IntStatus, 15) + 1*ReadBit(IntStatus, 14);
}

uint8_t VEML6030::SetWindow(unsigned int HighVal, unsigned int LowVal) //Set the high and low values for the interrupt window
{
	uint8_t ErrorHigh = WriteWord(WINDOW_H_CMD, HighVal);
	uint8_t ErrorLow = WriteWord(WINDOW_L_CMD, LowVal);
	if(ErrorHigh == ErrorLow && ErrorLow == 0) return 0;
	else return ErrorLow;
}

uint8_t VEML6030::SetPowerSaveMode(uint8_t Mode)  //Sets power save mode from 0-3
{	//Add calculation of global refresh time! 
	unsigned int PowerSaveMode = ReadWord(PWR_CMD);
	PowerSaveMode = (PowerSaveMode | 0x06) & (Mode << 1); //Adjust PSM, keep PSM_EN bit
	return WriteWord(PWR_CMD, PowerSaveMode);
}

uint8_t VEML6030::PowerSaveOn()
{
	unsigned int PowerSaveMode = ReadWord(PWR_CMD);
	return WriteWord(PWR_CMD, (PowerSaveMode | 0x01)); //Set PSM_EN bit, return error code
}

uint8_t VEML6030::PowerSaveOff()
{
	unsigned int PowerSaveMode = ReadWord(PWR_CMD);
	return WriteWord(PWR_CMD, (PowerSaveMode & 0xFFFE)); //Clear PSM_EN bit, return error code
}

uint8_t VEML6030::SendCommand(uint8_t Command)
{
    Wire.beginTransmission(ADR);
    Wire.write(Command);
    return Wire.endTransmission(false);
}

uint8_t VEML6030::WriteWord(uint8_t Command, unsigned int Data)  //Writes value to 16 bit register
{
	Wire.beginTransmission(ADR);
	Wire.write(Command); //Write Command value
	Wire.write(Data & 0xFF); //Write LSB
	Wire.write((Data >> 8) & 0xFF); //Write MSB
	uint8_t Error = Wire.endTransmission();
	return Error;
}

uint8_t VEML6030::WriteConfig(unsigned int NewConfig)
{
	Wire.beginTransmission(ADR);
	Wire.write(CONF_CMD);  //Write command code to Config register
	Wire.write(NewConfig & 0xFF);
	Wire.write((NewConfig >> 8) & 0xFF); 
	uint8_t Error = Wire.endTransmission();

	if(Error == 0) {
		Config = NewConfig; //Set global config if write was sucessful 
		return 0;
	}
	else return Error; //If write failed, return failure condition
}

int VEML6030::ReadByte(uint8_t Command, uint8_t Pos) //Send command value, and high/low byte to read, returns desired byte
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

int VEML6030::ReadWord(uint8_t Command)  //Send command value, returns entire 16 bit word
{
	uint8_t Error = SendCommand(Command);
	Wire.requestFrom(ADR, 2, true);
	uint8_t ByteLow = Wire.read();  //Read in high and low bytes (big endian)
	uint8_t ByteHigh = Wire.read();
	if(Error == 0) return ((ByteHigh << 8) | ByteLow); //If read succeeded, return concatonated value
	else return -1; //Return error if read failed
}

// bool VEML6030::ReadBit(unsigned int Data, uint8_t Pos)
// {
// 	return (Data >> Pos) & 0x01; 
// }

uint8_t VEML6030::ReadBit(unsigned int Data, uint8_t Pos)
{
	return (Data >> Pos) & 0x01; 
}