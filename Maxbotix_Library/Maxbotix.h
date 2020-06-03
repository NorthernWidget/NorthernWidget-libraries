/******************************************************************************
Maxbotix.h
Library for interfacing with Maxbotix rangefinders for use with enviromental sensing
Bobby Schulz @ Northern Widget LLC
Andy Wickert @ Northern Widget LLC
7/16/2018
https://github.com/NorthernWidget-Skunkworks/Maxbotix_Library

This script is used to interface to the Maxbotix sensors and return distance to target, along with other
statistics

Boss: "I just heard that light travels faster than sound. I'm wondering if I should shout when I speak, just so my lips appear to sync-up with my words.""
Dilbert (thought): "A little knowledge can be a ridiculous thing."
-Scott Adams

Distributed as-is; no warranty is given.
******************************************************************************/

#ifndef MAXBOTIX_h
#define MAXBOTIX_h

#include "Arduino.h"
#include <math.h>
#include <SoftwareSerial.h>

/////////////
// GLOBALS //
/////////////



// Define ranges here instead of in function, w/ pointer?

class Maxbotix
{
	public:
		Maxbotix(uint8_t DataPin = -1);
		bool begin(uint8_t _nPings=1, bool _writeAll=false, \
               uint8_t _ExPin=-1);
		// bool begin(uint8_t _RxPin);
		int16_t GetRange();
		String GetHeader();
		String GetString();

	private:
		uint8_t nPings = 1; //Fix??
		bool writeAll;
		uint8_t ExPin;

		uint16_t ranges[10] = {0}; //Fix hard code! and global??

    SoftwareSerial softSerial;  //Fix hardcode!
		// extern SoftwareSerial softSerial;
    void serialBufferClear();
    int32_t sum(uint16_t values[], uint8_t nvalues, bool errorNegative=true);
    float mean(uint16_t values[], uint8_t nvalues, bool errorNegative=true);
    float standardDeviation(uint16_t values[], uint8_t nvalues, float mean, \
                            bool errorNegative=true);
		
};

#endif
