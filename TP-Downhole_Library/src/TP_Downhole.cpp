/******************************************************************************
TP_Downhole.cpp
Library for TP-Downhole pressure and tempurature sensor, made by Northern Widget LLC.
Bobby Schulz @ Northern Widget LLC
5/9/2018
Hardware info located at: https://github.com/NorthernWidget/TP-DownHole

The TP-Downhole is a small form factor tempreture and pressure sensor which uses a combination of an 
MS5803 pressure sensor and an MCP3421 ADC in combination with a thermistor for high accuracy
tempreture measurment.

"Size matters not. Look at me. Judge me by my size do you? And well you should not. For my ally is the
Force, and a powerful ally it is"
-Yoda

Distributed as-is; no warranty is given.
******************************************************************************/

#include <Wire.h> // Wire library is used for I2C
#include "TP_Downhole.h"

TP_Downhole::TP_Downhole()
// Base library type I2C
{
}


uint8_t TP_Downhole::begin(model Model)
// Initialize library for subsequent pressure measurements
{
    switch(Model) {
        case(TP2v0):
            {
            PresMax = 2;
            PresAdr = 0x77;
            TempAdr = 0x6A;
            ThermB = 3435;
            VRef = 3.3;
            R0 = 47000;
            ThermR = 10000;
            float CoefTemp1[4] = {0.003354016, 0.0003074038, 1.019153E-05, 9.093712E-07};
            memcpy(ThermCoefs, CoefTemp1, 4);
            }
            break;

        case(TP2v1):
            {
            PresMax = 2;
            PresAdr = 0x77;
            TempAdr = 0x6A;
            ThermB = 3435;
            VRef = 3.3;
            R0 = 47000;
            ThermR = 10000;
            float CoefTemp2[] = {0.003354016, 0.0003074038, 1.019153E-05, 9.093712E-07};
            memcpy(ThermCoefs, CoefTemp2, 4);
            }
            break;

        case(TP5v1):
            {
            PresMax = 5;
            PresAdr = 0x77;
            TempAdr = 0x6A;
            ThermB = 3435;
            VRef = 3.3;
            R0 = 47000;
            ThermR = 47000;
            float CoefTemp3[] = {0.003354016, 0.0003074038, 1.019153E-05, 9.093712E-07};
            memcpy(ThermCoefs, CoefTemp3, 4);
            }
            break;

        case(TP2v2):
            {
            PresMax = 2;
            PresAdr = 0x77;
            TempAdr = 0x6A;
            ThermB = 3435;
            VRef = 1.8;
            R0 = 10000;
            ThermR = 10000;
            float CoefTemp4[] = {0.003354016, 0.0003074038, 1.019153E-05, 9.093712E-07};
            memcpy(ThermCoefs, CoefTemp4, 4);
            }
            break;

        case(TP5v2):
            {
            PresMax = 5;
            PresAdr = 0x77;
            TempAdr = 0x6A;
            ThermB = 3435;
            VRef = 1.8;
            R0 = 10000;
            ThermR = 10000;
            float CoefTemp4[] = {0.003354016, 0.0003074038, 1.019153E-05, 9.093712E-07};
            memcpy(ThermCoefs, CoefTemp4, 4);
            }
            break;

        case(TP2b2):
            {
            PresMax = 2;
            PresAdr = 0x76;
            TempAdr = 0x6A;
            ThermB = 3435;
            VRef = 0; //No external thermistor 
            }
            break;

        case(TP2A2):
            {
            PresMax = 2;
            PresAdr = 0x76;
            TempAdr = 0x6A;
            ThermB = 3435;
            VRef = 1.8;
            R0 = 10000;
            ThermR = 10000;
            float CoefTemp6[] = {0.003354016, 0.0003074038, 1.019153E-05, 9.093712E-07};
            memcpy(ThermCoefs, CoefTemp6, 4);
            }
            break;

        case(TP2a2):
            {
            PresMax = 2;
            PresAdr = 0x76;
            TempAdr = 0x6B;
            ThermB = 3435;
            VRef = 1.8;
            R0 = 10000;
            ThermR = 10000;
            float CoefTemp6[] = {0.003354016, 0.0003074038, 1.019153E-05, 9.093712E-07};
            memcpy(ThermCoefs, CoefTemp6, 4);
            }
            break;
    }
    pressure.reset();
    uint8_t ErrorPressure = pressure.begin(PresAdr, PresMax);


    
    uint8_t ErrorADC = adc.Begin(TempAdr);
    adc.SetResolution(ThermRes);

    if(ErrorADC == 0 && ErrorPressure == 0) return 0;
    else return -1; //Retun failure is both devices are not connected 
}

float TP_Downhole::getTemperature(uint8_t Location) //Returns temp in C from either subsensor
{
    if(Location == 0) return pressure.getTemperature(CELSIUS, TempRes);
    if(Location == 1) {
        adc.SetResolution(ThermRes);  //Make sure resolution is set on onboard ADC
        float Val = adc.GetVoltage(false); //Get a voltage conversion from ADC, allow use of stale values for faster sampling (since thermistor changes "very slowly")
        Val = VRef - Val; //Voltage is measure across thermistor, not relative to ground
        float Temp = TempConvert(Val, VRef, R0, ThermCoefs[0], ThermCoefs[1], ThermCoefs[2], ThermCoefs[3], ThermR);  //Make conversion to temp
        Temp = Temp - 273.15; //Convert from Kelvin to C
        return Temp;
    }
}


float TP_Downhole::getTemperature() //By default get thermistor temp value
{
    getTemperature(1); 
}


float TP_Downhole::getPressure()
// Return a pressure reading units mBar.
{
    return pressure.getPressure(PresRes);  //Sample at specified pressure resolution 
}

float TP_Downhole::TempConvert(float V, float Vcc, float R, float B, float R25){
  float Rt = (Vcc/V)*R - R;
  float T = 1/((1/B)*log(Rt/R25) + 1/298.15);
  return T;
}

float TP_Downhole::TempConvert(float V, float Vcc, float R, float A, float B, float C, float D, float R25){
  float Rt = (Vcc/V)*R - R;
  float LogRt = log(Rt/R25);
  float T = 1.0/(A + B*LogRt + C*pow(LogRt, 2.0) + D*pow(LogRt, 3.0));
  return T;
}

String TP_Downhole::GetHeader() 
{
    return "Pressure [mBar], Temp DH [C], Temp DHt [C], "; //return header string
}

String TP_Downhole::GetString()
{
    return String(getPressure()) + "," + String(getTemperature(0)) + "," + String(getTemperature(1)) + ","; //Return data string
}
