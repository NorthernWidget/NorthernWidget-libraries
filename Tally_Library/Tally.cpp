/******************************************************************************
Tally.cpp
Library for interfacing Tally event counter
Bobby Schulz @ Northern Widget LLC
1/22/2019
https://github.com/NorthernWidget-Skunkworks/Tally_Library

This library is used to interface to the Tally event counter module and allow for easy reading and resetting of the device

"At each increase of knowledge, as well as on the contrivance of every new tool, human labour becomes abridged."
-Charles Babbage

Distributed as-is; no warranty is given.
******************************************************************************/

#include "Tally.h"

Tally::Tally(uint8_t ClockPin_, uint8_t DataPin_, bool ClockLogic_)
{
	ClockPin = ClockPin_;  //Copy to global variables, pin used for clock output
	DataPin = DataPin_;  //Copy to global variables, pin used for data input
	ClockLow = ClockLogic_; //Copy to global variables, used to invert clock output to correct for hardware
}

uint8_t Tally::begin(uint8_t ClockPin_, uint8_t DataPin_, bool ClockLogic_)
{
	ClockPin = ClockPin_;  //Copy to global variables, pin used for clock output
	DataPin = DataPin_;  //Copy to global variables, pin used for data input
	ClockLow = ClockLogic_; //Copy to global variables, used to invert clock output to correct for hardware
	begin(); //Call standard begin
}		

uint8_t Tally::begin()
{
	pinMode(ClockPin, OUTPUT); //Setup pins
	pinMode(DataPin, INPUT);
	digitalWrite(ClockPin, ClockLow); //Set clock pin low
}

unsigned int Tally::GetTicks() //Read out number of ticks since last reset 
{
	unsigned int Data = 0; //Number of pulses since last read
	digitalWrite(ClockPin, !ClockLow);  //Pulse clock pin high for >5ms to load new data
	delay(5.5); 
  	digitalWrite(ClockPin, ClockLow); 

	for(int i = 0; i < 16; i++) {  //Clock out all 16 bits
    digitalWrite(ClockPin, !ClockLow); //Send rising edge
		delayMicroseconds(OnTime); //Wait to stabilize
		Data = (Data << 1) | digitalRead(DataPin);
    	digitalWrite(ClockPin, ClockLow); //Send falling edge
		delayMicroseconds(OffTime);
	}

	return Data;  //Return concatonated value
}

void Tally::ClearTicks() //Reset tally, clear counter 
{
	digitalWrite(ClockPin, !ClockLow); //Send 20ms clock pulse
	delay(9);
	digitalWrite(ClockPin, ClockLow); 
}

String Tally::GetString(bool ClearOnRead)
{
	uint16_t Val = 0; 
	Val = GetTicks(); //Get count from device
	if(ClearOnRead) ClearTicks(); //Reset device if directed to, TRUE by default
	return String(Val) + ","; //Cast value to string and return 
}

String Tally::GetHeader()
{
	return "Count [#], "; //FIX check if right syntax??
}


