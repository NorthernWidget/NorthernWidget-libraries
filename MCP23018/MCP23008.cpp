/******************************************************************************
MCP23008.cpp
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
#include <Arduino.h>
#include <MCP23008.h>
#include <Wire.h>

MCP23008::MCP23008(int _ADR)
{
  // ADR = _ADR; //FIX ADR!
  Wire.begin();  
}


int MCP23008::begin(void)  //FIX! Combine interrupt lines be default!
{

  PinModeConf = 0xFF; //Default to all inputs //FIX make cleaner

  Wire.beginTransmission(ADR); //Test if device present 
  if(Wire.endTransmission() != 0) return -1;
  else return 1;
}


int MCP23008::PinMode(int Pin, uint8_t PinType)
{
  if(Pin > 8 || Pin < 0)
  {
    return -1;  //Fail if pin out of range
  }

  if(PinType == INPUT)
  {
    PinModeConf = PinModeConf | (0x01 << Pin); //Set bit for input
    PullUpConf = PullUpConf & ~(0x01 << Pin);  //Clear bit for pullup 
    // Serial.print("Reg 0x03 = ");  //DEBUG!
    // Serial.println(PinModeConf, HEX); //DEBUG!
    SetDirection(PinModeConf);
    SetPullup(PullUpConf);
    return 1;
  }
  else if(PinType == OUTPUT)
  {
    PinModeConf = PinModeConf & ~(0x01 << Pin); //Clear bit for output
    PullUpConf = PullUpConf | (0x01 << Pin);  //Set pullup bit to allow for "push-pull" operation
    // Serial.print("Reg 0x03 = ");  //DEBUG!
    // Serial.println(PinModeConf, HEX); //DEBUG!
    // Serial.println(PullUpConf, HEX); //DEBUG!
    SetDirection(PinModeConf);
    SetPullup(PullUpConf);
    return 0;
  }

  else if(PinType == OPEN_DRAIN)
  {
    PinModeConf = PinModeConf & ~(0x01 << Pin); //Clear bit for output
    PullUpConf = PullUpConf & ~(0x01 << Pin);  //Clear pullup bit to allow for open drain operation
    // Serial.print("Reg 0x03 = ");  //DEBUG!
    // Serial.println(PinModeConf, HEX); //DEBUG!
    SetDirection(PinModeConf);
    SetPullup(PullUpConf);
    return 0;
  }

  else if(PinType == INPUT_PULLUP)
  {
    PinModeConf = PinModeConf | (0x01 << Pin); //Set bit for input
    PullUpConf = PullUpConf | (0x01 << Pin);  //Set bit for pullup  
    // Serial.print("Reg 0x03 = ");  //DEBUG!
    // Serial.println(PinModeConf, HEX); //DEBUG!
    SetDirection(PinModeConf); 
    SetPullup(PullUpConf);
    return 0;
  }
  else 
    return -1; //Fail if pin type not defined 
}

int MCP23008::DigitalWrite(int Pin, bool State)
{
  if(Pin > 8 || Pin < 0)
  {
    return -1; //Fail if pin out of range
  }

  if(State == HIGH)
  {
    PortState = PortState | (0x01 << Pin);
    // Serial.print("Reg 0x01 = ");  //DEBUG!
    // Serial.println(Port, HEX); //DEBUG!
    SetPort(PortState);
    return 1;
  }
  else if(State == LOW)
  {
    PortState = PortState & ~(0x01 << Pin);
    // Serial.print("Reg 0x01 = ");  //DEBUG!
    // Serial.println(Port, HEX); //DEBUG!
    SetPort(PortState);
    return 0;
  }
  else 
    return -1; //Fail if state is ill-defined
}

int MCP23008::SetInterrupt(int Pin, bool State)
{
  if(Pin > 8 || Pin < 0)
  {
    return -1; //Fail if pin out of range
  }

  if(State == ON)
  {
    InterruptConf = InterruptConf | (0x01 << Pin);
    // Serial.print("Reg 0x01 = ");  //DEBUG!
    // Serial.println(Port, HEX); //DEBUG!
    SetPort(InterruptConf);
    return 1;
  }
  else if(State == OFF)
  {
    InterruptConf = InterruptConf & ~(0x01 << Pin);
    // Serial.print("Reg 0x01 = ");  //DEBUG!
    // Serial.println(Port, HEX); //DEBUG!
    SetPort(InterruptConf);
    return 0;
  }
  else 
    return -1; //Fail if state is ill-defined
}

int MCP23008::SetPort(int Config) 
{
  Wire.beginTransmission(ADR); // transmit to device with address ADR
  Wire.write(LAT);   //Send to output set register
  Wire.write(Config);   
  // Serial.println(LATA + Port, HEX); //DEBUG!
  // Serial.println(Config, HEX); //DEBUG!
  // Serial.print("\n\n"); //DEBUG!
  return Wire.endTransmission();
}

int MCP23008::SetDirection(int Config) 
{
  // Serial.println(Config, HEX); //DEBUG!
  Wire.beginTransmission(ADR); // transmit to device with address ADR
  Wire.write(DIR);        //Send to port configuration register
  Wire.write(Config);              
  // Serial.println(DIRA + Port, HEX); //DEBUG!
  // Serial.print("\n\n"); //DEBUG!
  return Wire.endTransmission();    // stop transmitting
}

int MCP23008::SetPolarity(int Config) 
{
  Wire.beginTransmission(ADR); // transmit to device with address ADR
  Wire.write(POL);        //Send to port configuration register
  Wire.write(Config);              
  return Wire.endTransmission();    // stop transmitting
}

int MCP23008::SetPullup(int Config) 
{
  // Serial.println(Config, HEX); //DEBUG!
  Wire.beginTransmission(ADR); // transmit to device with address ADR
  Wire.write(PULLUP);        //Send to port configuration register
  Wire.write(Config);         
  return Wire.endTransmission();    // stop transmitting
}

int MCP23008::SetInt(int Config) 
{
  Wire.beginTransmission(ADR); // transmit to device with address ADR
  Wire.write(PULLUP);        //Send to port configuration register
  Wire.write(Config);         
  return Wire.endTransmission();    // stop transmitting
}

//IN DEVELOPMENT
int MCP23008::ReadPort(int Config)
{
  Wire.beginTransmission(ADR); // transmit to device with address ADR
  Wire.write(0x01);        //Send to port configuration register
  Wire.write(Config);              //Set port 1-4 as OUTPUT, all others as inputs
  return Wire.endTransmission();    // stop transmitting
}





