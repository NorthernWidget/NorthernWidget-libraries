/******************************************************************************
VEML6030.h
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

#ifndef VEML6030_h
#define VEML6030_h

#include <Arduino.h>
// #include <cmath.h>

//Commands
#define CONF_CMD 0x00
#define WINDOW_H_CMD 0x01
#define WINDOW_L_CMD 0x02
#define PWR_CMD 0x03
#define ALS_CMD 0x04
#define WHITE_CMD 0x05
#define INT_CMD 0x06

//Integration Times bit masks
#define IT25 0x300  //25ms
#define IT50 0x200  //50ms
#define IT100 0x000 //100ms
#define IT200 0x040 //200ms
#define IT400 0x080 //400ms
#define IT800 0x0C0 //800ms

#define GAIN_1 0x0000 //Gain x1
#define GAIN_2 0x0800 //Gain x2
#define GAIN_1_8 0x1000 //Gain x1/8
#define GAIN_1_4 0x1800 //Gain x1/4

//Persistance will define how man consecutive values required to trigger interrupt
#define PER_1 0x00 //Persistance of 1
#define PER_2 0x10 //Persistance of 2
#define PER_4 0x20 //Persistance of 4
#define PER_8 0x30 //Persistance of 8


class VEML6030
{
public:
    VEML6030();  // Constructor
    uint8_t begin(uint8_t _ADR = 0x48); // Begin sensor with default address
    uint8_t SetIntTime(unsigned int TimeBits);
    unsigned int GetIntTime();
    unsigned int IntTime2Bits(unsigned int Time);
    uint8_t SetGain(unsigned int GainVal);
    float GetGain();
    float GetGain(uint8_t GainBits);
    unsigned int Gain2Bits(float GainVal);
    uint8_t Shutdown();
    uint8_t PowerOn();
    uint8_t IntOn();
    uint8_t IntOff();
    uint8_t SetPersistence(uint8_t Persistance);
    uint8_t AutoRange();
    float GetLux();
    unsigned int GetALS();
    unsigned int GetWhite();
    int GetIntStatus();
    uint8_t SetWindow(unsigned int HighVal, unsigned int LowVal);
    uint8_t SetPowerSaveMode(uint8_t Mode);
    uint8_t PowerSaveOn();
    uint8_t PowerSaveOff();


private:
    uint8_t SendCommand(uint8_t Command);
    uint8_t WriteWord(uint8_t Command, unsigned int Data);
    uint8_t WriteConfig(unsigned int NewConfig);
    int ReadByte(uint8_t Command, uint8_t Pos);
    int ReadWord(uint8_t Command);
    // bool ReadBit(unsigned int Data, uint8_t Pos);
    uint8_t ReadBit(unsigned int Data, uint8_t Pos);
    uint8_t ADR = 0x48; //Default to 0x48 
    unsigned int Config = 0; //Global config value
    float Gain = 0; //Global gain value
    unsigned int IntTime = 0; //Global integration time 
    float GainVals[4] = {1, 2, 0.125, 0.25}; //Used to go from bit field to arbitrary gain value
    uint8_t GainValBits[4] = {0b10, 0b11, 0b00, 0b01}; //Bit values for gains, 1/8x, 1/4x, 1x, 2x
};

#endif
