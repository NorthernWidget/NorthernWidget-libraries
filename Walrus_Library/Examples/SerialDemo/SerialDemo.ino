/******************************************************************************
SerialDemo.ino
Basic serial demo script for TP-Downhole pressure and tempurature sensor, made by Northern Widget LLC.
Bobby Schulz @ Northern Widget LLC
2/28/2020
Hardware info located at: https://github.com/NorthernWidget/TP-DownHole

This code reads temperature and pressure data from the Walrus over I2C and diplays it via serial

"Size matters not. Look at me. Judge me by my size do you? And well you should not. For my ally is the
Force, and a powerful ally it is"
-Yoda

Distributed as-is; no warranty is given.
******************************************************************************/

#include <Walrus_I2C.h>

Walrus Sensor; //Instantiate Walrus instance 

const unsigned long UpdatePeriod = 5000; //Update every 5 seconds

void setup()
{
	Sensor.begin(); //Initialize sensor
	Serial.begin(9600); //Initialize serial for basic communication 
	Serial.print("Begin Walrus Demo...\n\n"); 
	Serial.println(Sensor.GetHeader()); //Print header from Walrus to identify values
}

void loop()
{
	static unsigned long LocalTime = millis(); //Initialize local time keeping
	if((millis() - LocalTime) > UpdatePeriod) { //If a period has passed, print new values
		Serial.println(Sensor.GetString()); //Grab pre-compiled string from sensor and print
	}
}