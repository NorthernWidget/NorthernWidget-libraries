/******************************************************************************
MCP3421.h
Library for MCP3421 ADC
Bobby Schulz @ Northern Widget LLC & UMN
2/20/2018

MCP3421 is a 18 bit ADC, PGA, and various modes of resolution operation in order to facilitate various data rates

"Simplicity is prerquisite for reliability"
-Edsger W. Dijkstra

Distributed as-is; no warranty is given.
******************************************************************************/

#ifndef MCP3421_h
#define MCP3421_h

#include "Arduino.h"

#define CONTINUIOUS 1
#define SINGLE_SHOT 0

class MCP3421
{
  public:

    MCP3421(int _ADR);
    MCP3421(void);

    int Begin(int _ADR);
    
    int Begin(void);

    long GetVoltageRaw(bool WaitForVal = true); //Force new values by default  

    float GetVoltage(bool WaitForVal = false); //Force new values by default

    int SetGain(int DesiredGain);

    int SetResolution(int DesiredResolution);

    int SetMode(int Mode);

  private:
    int ADR;
    int GetConfig();
    int measurement_duration_ms = 5; // time to wait to complete measurement
                                     // default to 12 bits

};

#endif

