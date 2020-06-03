#ifndef TCA9534_h
#define TCA9534_h

#include "Arduino.h"

class TCA9534
{
    //     enum IO_Type
    // {
    //     INPUT,
    //     OUTPUT
    // };

  public:
    TCA9534(int _ADR);
    int Begin(void);
    int PinMode(int Pin, boolean PinType);
    int DigitalWrite(int Pin, boolean State);

  private:
    int ADR;
  	uint8_t PinModeConf = 0xFF; //All pins natively inputs
    uint8_t Port = 0x00; //All pins natively off
    uint8_t PinPolarityConfig = 0x00; //All pins natively non-inverted

    int SetPort(int Config);
    int SetDirection(int Config);
    int SetPolarity(int Config);
    int ReadPort(int Config); //IN DEVELOPMENT
};

#endif