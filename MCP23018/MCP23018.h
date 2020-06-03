/******************************************************************************
MCP23018.h
Interface for MCP23018 16 bit IO expander
Bobby Schulz @ Northern Widget LLC
07/18/2019
https://github.com/NorthernWidget-Skunkworks/MCP23018

Allows control of all aspects of the control of the IO expander 

0.0.0

"All existing things are really one"
-Zhuangzi

Distributed as-is; no warranty is given.
******************************************************************************/

#ifndef MCP23018_h
#define MCP23018_h

#include "Arduino.h"

#define ON 1
#define OFF 0

#define A 0
#define B 1

#define OPEN_DRAIN 3  //4th potential state of operation for IO expander pins

#define BASE_ADR 0x20 //Base address for device, to be or-ed with configurable address

#define DIRA 0x00
#define DIRB 0x01
#define POLA 0x02 
#define POLB 0x03
#define INTENA 0x04
#define INTENB 0x05
#define PULLUPA 0x0C
#define PULLUPB 0x0D
#define PORTA 0x12
#define PORTB 0x13
#define LATA 0x14
#define LATB 0x15

#define DEFVALA 0x06
#define DEFVALB 0x07
#define IOCON 0x0A //Also 0x0B
#define FLAGA 0x0E
#define FLAGB 0x0F
#define INTCAPA 0x10
#define INTCAPB 0x11
#define INTCONA 0x08
#define INTCONB 0x09

class MCP23018
{
    //     enum IO_Type
    // {
    //     INPUT,
    //     OUTPUT
    // };

  public:
    MCP23018(int _ADR);
    int begin(void);
    int PinMode(int Pin, uint8_t State, bool Port);
    int DigitalWrite(int Pin, bool State, bool Port);
    int SetInterrupt(int Pin, bool State, bool Port);

  private:
    const int ADR = BASE_ADR; //FIX! Replace with equation later
  	uint8_t PinModeConf[2] = {0xFF}; //All pins natively inputs
    uint8_t PortState[2] = {0}; //All pins natively off
    // uint8_t PortAState = 0x00; //All pins natively off
    // uint8_t PortBState = 0x00; //All pins natively off
    uint8_t PinPolarityConfig[2] = {0x00}; //All pins natively non-inverted
    uint8_t PullUpConf[2] = {0x00}; //Natively disabled
    uint8_t InterruptConf[2] = {0x00}; //Interrupts dissabled by default

    int SetPort(int Config, bool Port);
    int SetDirection(int Config, bool Port);
    int SetPolarity(int Config, bool Port);
    int SetPullup(int Config, bool Port);
    int SetInt(int Config, bool Port);
    int ReadPort(int Config, bool Port); //IN DEVELOPMENT
};

#endif