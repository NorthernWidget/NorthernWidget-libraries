/******************************************************************************
TP_Downhole.h
Library for TP-Downhole pressure and tempurature sensor, made by Northern Widget LLC.
Bobby Schulz @ Northern Widget LLC
5/9/2018
Hardware info located at: https://github.com/NorthernWidget/TP-DownHole

The TP-Downhole is a small form factor tempreture and pressure sensor which uses a combination of an 
MS5803 pressure sensor and an MCP3421 ADC in combination with a thermistor for high accuracy
tempreture measurment.

"Size matter not. Look at me. Judge me by my size do you? And well you should not. For my ally is the
Force, and a powerful ally it is"
-Yoda

Distributed as-is; no warranty is given.
******************************************************************************/

#ifndef TP_Downhole_h
#define TP_Downhole_h

#include <Arduino.h>
#include <MCP3421.h>
#include <MS5803.h>

//Models
enum model
{
    TP2v0 = 0x0200,
    TP2v1 = 0x0201,
    TP5v1 = 0x0501,
    TP2v2 = 0x0202,
    TP5v2 = 0x0502,
    TP2b2 = 0x2B02,
    TP2A2 = 0x2A02,
    TP2a2 = 0x2A03
};

class TP_Downhole
{
public:
    TP_Downhole();  // Constructor
    MS5803 pressure;
    MCP3421 adc; //Initialize MCP3425 with default address, 0x6A
    uint8_t begin(model Model); // Collect model value

    // Return calculated temperature from sensor
    float getTemperature(uint8_t Location);
    float getTemperature();
    // Return calculated pressure from sensor
    float getPressure();
    String GetHeader();
    String GetString();

    uint8_t ThermRes = 18; //Use 18 bits by default
    precision PresRes = ADC_4096; //Use 12 bit resolution by default
    precision TempRes = ADC_512; //Use 11 bit resolution by default

private:
    uint8_t PresMax = 2; //Maximum pressure for the sensor used
    uint8_t PresAdr = 0x77; //The address used by the MS5803 part of the TP-Downhole
    uint8_t TempAdr = 0x6A; //the address used by the MCP3421 ADC on board the TP-Downhole
    float ThermB = 3435; //Basic B value for thermistor 
    float ThermCoefs[4] = {0.003354016, 0.0003074038, 1.019153E-05, 9.093712E-07}; //Coefficients for enhanced accuracy
    float VRef = 1.8; //Voltage referance used for thermistor
    float R0 = 10000; //Series resistor value with thermistor 
    float ThermR = 10000; //Nominal resistace value for the thermistor 
    float TempConvert(float V, float Vcc, float R, float B, float R25);
    float TempConvert(float V, float Vcc, float R, float A, float B, float C, float D, float R25);
};

#endif
