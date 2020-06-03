/******************************************************************************
Tally.h
Library for interfacing Tally event counter
Bobby Schulz @ Northern Widget LLC
1/22/2019
https://github.com/NorthernWidget-Skunkworks/Tally_Library

This library is used to interface to the Tally event counter module and allow for easy reading and resetting of the device

"At each increase of knowledge, as well as on the contrivance of every new tool, human labour becomes abridged."
-Charles Babbage

Distributed as-is; no warranty is given.
******************************************************************************/

#ifndef TALLY_h
#define TALLY_h

#include "Arduino.h"

#define CLOCK_DEFAULT 11
#define DATA_DEFAULT 12

class Tally
{
  public:
    Tally(uint8_t ClockPin_ = CLOCK_DEFAULT, uint8_t DataPin_ = DATA_DEFAULT, bool ClockLogic_ = LOW); //Use positive logic and default pins
    uint8_t begin(uint8_t ClockPin_, uint8_t DataPin_, bool ClockLogic_ = LOW);
    uint8_t begin(void);
    unsigned int GetTicks(void);
    void ClearTicks(void);
    String GetString(bool ClearOnRead = true);
    String GetHeader(); 

  private:
    uint8_t ClockPin = CLOCK_DEFAULT;
    uint8_t DataPin = DATA_DEFAULT;
    bool ClockLow = LOW; 
    unsigned int OnTime = 500; //On time of clocking pulse in microseconds 
    unsigned int OffTime = 500; //Off time of clocking pulse in microseconds  
};

#endif