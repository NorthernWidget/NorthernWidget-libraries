/**************************************************************************/
/*! 
    @file     MCP4725.cpp
    @author   K.Townsend (Adafruit Industries)
	@license  BSD (see license.txt)
	
	I2C Driver for Microchip's MCP4725 I2C DAC

	This is a library for the Adafruit MCP4725 breakout
	----> http://www.adafruit.com/products/935
		
	Adafruit invests time and resources providing this open source code, 
	please support Adafruit and open-source hardware by purchasing 
	products from Adafruit!

	@section  HISTORY

    v1.0 - First release
*/
/**************************************************************************/
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <Wire.h>

#include "MCP4725.h"

/**************************************************************************/
/*! 
    @brief  Instantiates a new MCP4725 class
*/
/**************************************************************************/
MCP4725::MCP4725() {
}

/**************************************************************************/
/*! 
    @brief  Setups the HW
*/
/**************************************************************************/
void MCP4725::begin(uint8_t _ADR) {
  ADR = _ADR;
  Wire.begin();

}
 
/**************************************************************************/
/*! 
    @brief  Sets the output voltage to a fraction of source vref.  (Value
            can be 0..4095)

    @param[in]  output
                The 12-bit value representing the relationship between
                the DAC's input voltage and its output voltage.
    @param[in]  writeEEPROM
                If this value is true, 'output' will also be written
                to the MCP4725's internal non-volatile memory, meaning
                that the DAC will retain the current voltage output
                after power-down or reset.
*/
/**************************************************************************/
uint8_t MCP4725::setVoltage( uint16_t output, bool writeEEPROM )
{
#ifdef TWBR  //REMOVE??
  uint8_t twbrback = TWBR;
  TWBR = ((F_CPU / 400000L) - 16) / 2; // Set I2C frequency to 400kHz
#endif
  Wire.beginTransmission(ADR);  //ADD EEPROM FEATURE!!!!!!! FIX! 
  // if (writeEEPROM)
  // {
  //   Wire.write(MCP4726_CMD_WRITEDACEEPROM);
  // }
  // else
  // {
  //   Wire.write(MCP4726_CMD_WRITEDAC);
  // }
  Wire.write(Config | REG_CMD_MASK); //Write global config, OR with command bit mask
  Wire.write(output / 16);                   // Upper data bits          (D11.D10.D9.D8.D7.D6.D5.D4)
  Wire.write((output % 16) << 4);            // Lower data bits          (D3.D2.D1.D0.x.x.x.x)
  uint8_t Status = Wire.endTransmission();
#ifdef TWBR
  TWBR = twbrback;
#endif
return Status; //Return result of I2C transmission
}

uint8_t MCP4725::Sleep(bool State, uint8_t Level) 
{
  if(State) {
    Config = Config & 0xF9; //Clear sleep bits
    Config = Config | (Level << 1); //Set new value
  }
  else {
    Config = Config & 0xF9; //Clear sleep bits, put device into normal mode
  }

  // Serial.println(Config, HEX); //DEBUG!
  Wire.beginTransmission(ADR);
  Wire.write(Config | CONFIG_CMD_MASK);
  return Wire.endTransmission(); //Send sucess result 
}

uint8_t MCP4725::SetGain(bool State) 
{
  if(State) Config = Config | 0x01; //Set gain bit (2x mode)
  else Config = Config & 0xFE;; //Clear gain bit (1x mode)

  Wire.beginTransmission(ADR);
  Wire.write(Config | CONFIG_CMD_MASK);
  return Wire.endTransmission(); //Send sucess result 
}

uint8_t MCP4725::SetRef(uint8_t State)
{
  Config = Config & 0xE7; //Clear VRef/buffer select bits
  Config = Config | (State << 3); //Set appropriate buffer/vref source values

  Wire.beginTransmission(ADR);
  Wire.write(Config | CONFIG_CMD_MASK);
  return Wire.endTransmission(); //Send sucess result 
}
