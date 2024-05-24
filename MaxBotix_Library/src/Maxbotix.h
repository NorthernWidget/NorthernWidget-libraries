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

/**
 * @class Maxbotix
 * @brief Arduino library to enable control and communication with MaxBotix 
 * ultrasonic rangefinders using a software serial implementation.
 * @details This Arduino library will enable the microcontroller to control and
 * read output from a MaxBotix ultrasonic rangefinder. It works using
 * software serial, and is not programmed to interface wtih the pulse-width
 * or analog-voltage outputs of the Maxbotix rangefinder. It can average
 * multiple measurements to obtain a more robust distance to its target.
 *
 *  * \verbatim [![DOI](https://zenodo.org/badge/141178473.svg)](https://zenodo.org/badge/latestdoi/141178473) \endverbatim
*/
class Maxbotix
{
	public:
		Maxbotix(uint8_t DataPin = -1);

    /**
     * @brief
     * Sets globals and initializes software serial
     *
     * @details
     * Sets global variables required for a SoftwareSerial interface to record
     * data from a MaxBotix ultrasonic rangefinder. Initializes software serial
     * based on _RxPin.
     *
     * @param _RxPin Pin for SoftwareSerial receive at 9600 bps.
     *
     * @param _npings Number of pings over which you average; each ping itself
     * includes ten short readings that the sensor internally processes.
     * Must be > 0, obviously, and <= 255. Defaults to "1"
     *
     * @param _writeAll will write each reading of the sensor (each ping)
     * to the serial monitor and SD card. This is relevant only if npings > 1
     *
     * @param _ExPin Excitation pin that turns the sensor on; defaults to "-1",
     * assuming that the sensor is either always on or is being switched via
     * its main power supply.
     *
     * @param _RS232 defaults false for standard (TTL) logic; true for inverse
     * (RS232-style) logic. It works at standard logger voltages: this is not
     *  true RS232, but this is what MaxBotix calls it.
     *
     * @param _minRange_mm Minimum sensor range in mm; defaults to 501
     *
     * @param _maxRange_mm Maximum sensor range in mm; defaults to 4999
     *
     * Example:
     * ```
     * // SoftwareSerial with RxPin 7, averaging over 10 pings, and otherwise
     * // using default settings
     * alog.maxbotixHRXL_WR_Serial(7, 10);
     * ```
     *
     */
		bool begin(uint8_t _nPings=1, bool _writeAll=false, \
               uint8_t _ExPin=-1);

     /**
      * @brief
      * Returns the result of a single range measurement -- now set for multiple?
      *
      * @details
      * Returns the result of a single range measurement.
      * Communications error value = -9999
      * Internal error value (including range too long) = 5000
      * Range too short error value = 500
      * This code makes these internal error values negative to more easily
      * sort them out of the real results
      *
      */
		int16_t getRange();

    /**
     * @brief
     * Creates the appropritae header for the data file
     *
     * @details
     * Creates the appropritae header for the data file, based on
     * provided inputs (number of pings, recording all pings)
     *
     */
		String getHeader();


    /**
     * @brief
     * Returns the measurement result(s) as a String object
     *
     * @details
     * Returns the measurement result(s) as a String object
     *
     */
		String getString();

    // Included for backwards compatibility

    /**
     * @brief CapitalCase version of getRange() for backwards compatibility
     */
		int16_t GetRange();

    /**
     * @brief CapitalCase version of getHeader() for backwards compatibility
     */
		String GetHeader();

    /**
     * @brief CapitalCase version of getString() for backwards compatibility
     */
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
