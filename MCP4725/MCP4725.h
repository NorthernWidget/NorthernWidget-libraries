/**************************************************************************/
/*! 
    @file     Adafruit_MCP4725.h
    @author   K. Townsend (Adafruit Industries)
	@license  BSD (see license.txt)
	
	This is a library for the Adafruit MCP4725 breakout board
	----> http://www.adafruit.com/products/935
	
	Adafruit invests time and resources providing this open source code, 
	please support Adafruit and open-source hardware by purchasing 
	products from Adafruit!

	@section  HISTORY

    v1.0  - First release
*/
/**************************************************************************/

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <Wire.h>

//Control Reg | VRef1 | VRef0 | PD1 | PD0 | G | RDY/BSY | POR |

// #define MCP4726_CMD_WRITEDAC            (0x40)  // Writes data to the DAC
// #define MCP4726_CMD_WRITEDACEEPROM      (0x60)  // Writes data to the DAC and the EEPROM (persisting the assigned value after reset)

#define CONFIG_CMD_MASK 0x80 //Write values to config register 
#define REG_CMD_MASK 0x40 //Write values to registers

#define PD_640K 0b11
#define PD_125K 0b10
#define PD_1K 0b01

#define UNBUFFERED_VDD 0b00
#define BUFFERED_VREF 0b11
#define UNBUFFERED_VREF 0b10

#define GAIN_1X 0
#define GAIN_2X 1

#define ON 1
#define OFF 0

class MCP4725{
 public:
  MCP4725();
  void begin(uint8_t _ADR);  
  uint8_t setVoltage( uint16_t output, bool writeEEPROM = false);
  uint8_t Sleep(bool State, uint8_t Level = PD_640K);
  uint8_t SetGain(bool State);
  uint8_t SetRef(uint8_t State);

 private:
  uint8_t ADR;
  uint8_t Config; //Global config used to restore if a reset has occoured //FIX! Implement this feature!
};
