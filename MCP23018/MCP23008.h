/******************************************************************************
MCP23008.h
Interface for MCP23008 8 bit IO expander
Bobby Schulz @ Northern Widget LLC
10/18/2019
https://github.com/NorthernWidget-Skunkworks/MCP23018

Allows control of all aspects of the control of the IO expander 

0.0.0

"All existing things are really one"
-Zhuangzi

Distributed as-is; no warranty is given.
******************************************************************************/

#ifndef MCP23008_h
#define MCP23008_h

#include "Arduino.h"

#define ON 1
#define OFF 0

#define OPEN_DRAIN 3  //4th potential state of operation for IO expander pins

#define BASE_ADR 0x20 //Base address for device, to be or-ed with configurable address

#define DIR 0x00
#define POL 0x01
#define INTEN 0x02
#define PULLUP 0x06
#define PORT 0x09
#define LAT 0x0A

#define DEFVAL 0x04
#define IOCON 0x0A //Also 0x0B
#define FLAG 0x07
#define INTCAP 0x08
#define INTCON 0x05

class MCP23008
{
    //     enum IO_Type
    // {
    //     INPUT,
    //     OUTPUT
    // };

  public:
    MCP23008(int _ADR);
    int begin(void);
    int PinMode(int Pin, uint8_t State);
    int DigitalWrite(int Pin, bool State);
    int SetInterrupt(int Pin, bool State);

  private:
    const int ADR = BASE_ADR; //FIX! Replace with equation later
  	uint8_t PinModeConf = 0xFF; //All pins natively inputs
    uint8_t PortState = 0; //All pins natively off
    // uint8_t PortAState = 0x00; //All pins natively off
    // uint8_t PortBState = 0x00; //All pins natively off
    uint8_t PinPolarityConfig = 0x00; //All pins natively non-inverted
    uint8_t PullUpConf = 0x00; //Natively disabled
    uint8_t InterruptConf = 0x00; //Interrupts dissabled by default

    int SetPort(int Config);
    int SetDirection(int Config);
    int SetPolarity(int Config);
    int SetPullup(int Config);
    int SetInt(int Config);
    int ReadPort(int Config); //IN DEVELOPMENT
};

#endif