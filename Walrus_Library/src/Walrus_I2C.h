/******************************************************************************
Walrus.cpp
Library for Walrus pressure and tempurature sensor, made by Northern Widget LLC.
Based off of the TP-Downhole
Bobby Schulz @ Northern Widget LLC
5/9/2018
Hardware info located at:
https://github.com/NorthernWidget-Skunkworks/Project-Walrus

Distributed as-is; no warranty is given.
******************************************************************************/

#ifndef Walrus_I2C_h
#define Walrus_I2C_h

#include <Arduino.h>

#define TEMP_REG_0 0x06 //Location of external temp val on Walrus
#define TEMP_REG_1 0x09 //Location of MS5803 temp val on Walrus
#define TEMP_OFFSET 0x03 //Define length (offset) of tempurate vals in bytes
#define PRES_REG 0x03 //Location of pressure register on Walrus

#define ADR_DEFAULT 0x0D //Define default address

/**
 * @class Walrus: .
 * @brief Class to interface with the Walrus submersible temperature and
 * presure sensor
 * @details The Walrus is an encapsulated submersible
 * pressure and temperature sensor intended for water-level or barometric
 * monitoring.
 */
class Walrus
{
    public:

        /**
         * @brief Instantiate Walrus object
         */
        Walrus();  // Constructor

        /**
         * @brief Begin communications with the Walrus using a prescribed
         * address.
         * @param Address_: I2C address of Walrus
         * DOES NOT YET USE A VARIABLE ADDRESS!
         * THIS DEFAULT ADDRESS CURRENTLY CLASHES WITH HAAR'S DEFAULT!
         */
        uint8_t begin(uint8_t Address_ = ADR_DEFAULT);

        /**
         * @brief Return calculated temperature from Walrus.
         * @details This calculated temperature can be from either
         * the MS5803 sensor, which primarily measures pressure, or from the
         *
         * @param Location
         * TEMP_REG_0: Read dedicated temperature sensor.
         * TEMP_REG_1: Read temperature sensor withn the MS5803.
         * APPEARS TO JUST WORK WIHT TEMP_REG_0 FOR NOW
         */
        float getTemperature(uint8_t Location);

        /**
         * @brief Return calculated temperature from sensor, using a predefined
         * offset.
         * WHAT DOES THIS OFFSET DO?
         */
        float getTemperature();

        /**
         * @brief Return calculated pressure from sensor.
         * @details This is the MS5803 sensor, which can come in a variety
         * of different pressure ranges and sensitivities.
         */
        float getPressure();

        /**
         * @brief Return header
         * @details "Pressure [mBar],Temp DH [C],Temp DHt [C],"
         */
        String GetHeader();

        /**
         * @brief Return measurement data as a string
         * @details String(getPressure()) + "," + String(getTemperature(0))
         + "," + String(getTemperature(1)) + ",";
         */
        String GetString();

    private:
        uint8_t ADR = ADR_DEFAULT; //Default address
};

#endif
