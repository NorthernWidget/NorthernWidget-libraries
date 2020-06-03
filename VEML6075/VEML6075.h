/******************************************************************************
VEML6075.h
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

#ifndef VEML6075_h
#define VEML6075_h

#include <Arduino.h>
// #include <cmath.h>

//Commands
#define CONF_CMD 0x00
#define UVA_CMD 0x07
#define UVB_CMD 0x09
#define COMP1_CMD 0x0A
#define COMP2_CMD 0x0B
#define ID_CMD 0x0C

//Integration Times bit masks
#define IT50 0x0F  //50ms
#define IT100 0x1F //100ms
#define IT200 0x2F //200ms
#define IT400 0x3F //400ms
#define IT800 0x4F //800ms

#define CONTINUIOUS 0x00
#define SINGLE_SHOT 0x01

// #define ADR 0x10


class VEML6075
{
public:
    VEML6075();  // Constructor
    uint8_t begin(uint8_t ADR_); // Begin sensor with non-standard address
    uint8_t begin(); // Begin sensor
    uint8_t SetIntTime(unsigned int Time);
    uint8_t Shutdown();
    uint8_t PowerOn();
    uint8_t Mode(uint8_t OperatingMode);
    uint8_t StartConversion();
    float GetUVA();
    float GetUVB();


private:
    uint8_t SendCommand(uint8_t Command);
    uint8_t WriteConfig(uint8_t NewConfig);
    int ReadByte(uint8_t Command, uint8_t Pos);
    int ReadWord(uint8_t Command);
    uint8_t Config = 0; //Global config value
    long StartTime = 0; //Global conversion start time
    //Coefficients found by Vishay in application note
    float a = 1.92;
    float b = 0.55;
    float c = 2.46;
    float d = 0.63;
    uint8_t ADR = 0x10; //Use default address natively 
};

#endif
