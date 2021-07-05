
#ifndef HAAR_h
#define HAAR_h

#include "Arduino.h"
#include <Wire.h>

enum Sensor {
	RH_Sense = 0,
	Pres_Sense = 1
};

#define CTRL1 0x00
#define TEMP_PRES 0x02
#define TEMP_RH 0x04
#define PRES_REG 0x06
#define RH_REG 0x09

#define ON 1
#define OFF 0

/**
 * @class Haar
 * @brief Library for the rugged temperature, pressure, relative-humidity sensor
 * @details Project Haar, named for the cold fog off the north sea, is a
 * pressure, temperature, and relative-humidity sensor that can withstand
 * full submersion. Bobby Schulz designed it after the consistently-near-0C
 * temperatures and 100% relative humidity of Chimborazo Volcano, Ecuador,
 * claimed the lives of many brave but misguided BME-280 units, who then
 * catistrophically bricked their I2C buses and those of their associated
 * data loggers, thus taking down a large fraction of our hydromet network.
 *
 * *May their silicon and copper souls join the chorus of the stars.*
 *
 * \verbatim [![DOI](https://zenodo.org/badge/200929328.svg)](https://zenodo.org/badge/latestdoi/200929328) \endverbatim
 */
class Haar
{
	public:
	  /**
	   * @brief Instantiate the Haar sensor class
	   */
		Haar();

	  /**
	   * @brief Begin communications with the Haar sensor.
	   * @param[in] ADR_: I2C address. Defaults to 0x42.
	   */
		uint8_t begin(uint8_t ADR_ = 0x42);

	  /**
	   * @brief Return the currently stored pressure [in mBar]
	   * @param[in] update: Read and store a new pressure value before sending?
	   * By default is false; simply returns the already-available pressure value.
	   */
		float getPressure(bool update = false);

	  /**
	   * @brief Return the currently stored relative humidity [%]
	   * @param[in] update: Read and store a new pressure value before sending?
	   * By default is false; simply returns the already-available RH value.
	   */
		float getHumidity(bool update = false);

	  /**
	   * @brief Return the currently stored Temperature [degrees C]
	   * @param[in] Device: Selects which sensor is used to measure the
	   * temperature. Input can be `RH_Sense`, which corresponds to a 0, or
	   * `Pres_Sense`, which corresponds to a 1. By default, this is RH_Sense,
	   * as this sensor has a better internal temperature sensor.
	   * @param[in] update: Read and store a new pressure value before sending?
	   * By default is false; simply returns the already-available temperature
     * value.
	   */
		float getTemperature(Sensor Device = RH_Sense, bool update = false);

	  /**
	   * @brief Enable or disable device sleep mode
	   * THIS FUNCTION CURRENTLY DOES NOTHING!
	   * @param State: Can be `ON` (1) or `OFF` (0). By default, this command sets
	   * "Sleep" to ON, sending the device into a low-power sleep mode.
	   */
		uint8_t sleep(bool state = ON);

	  /**
	   * @brief Return a new sample of all of the data.
	   * @param Block: if `true`, wait for data to be returned.
	   * Defaults to `false`.
	   */
		uint8_t updateMeasurements(bool block = false); //Default to non-blocking

	  /**
	   * @brief Checks for updated data. Returns `true` if new data are available;
	   * otherwise returns `false`
	   * @param Block: if `true`, wait for data to be returned. Defaults to
	   * `false`.
	   */
		bool newData();

	  /**
	   * @brief The most important function for the user! Returns all data as a
	   * comma-separated string: "P,RH,T(P),T[RH],".
	   * @details This string is: PRESSURE,RELATIVE_HUMIDITY,
	   * TEMPERATURE_FROM_PRESSURE_SENSOR,TEMPERATURE_FROM_RH_SENSOR,
	   */
		String getString();

	  /**
	   * @brief Returns a header:
     * "Pressure Atmos [mBar], Humidity [%], Temp Pres [C], Temp RH [C],"
	   */
		String getHeader();

	private:
		uint8_t ADR = 0x42; //Default global sensor address
		int16_t getWord(uint8_t Reg);
		unsigned long timeoutGlobal = 500; //Timeout value, ms //FIX??
		bool dataRequested = false; //Flag for keeping track of data requests and
		                            // data retrevals
};

#endif
