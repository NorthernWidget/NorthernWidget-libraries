#include <Arduino.h>
#include <TCA9534.h>
#include <Wire.h>

TCA9534::TCA9534(int _ADR)
{
  ADR = _ADR; 
  Wire.begin();  
}


int TCA9534::Begin(void)
{
  Wire.beginTransmission(ADR); //Test if device present 
  if(Wire.endTransmission() != 0) return -1;
  else return 1;
}


int TCA9534::PinMode(int Pin, boolean PinType)
{
  if(Pin > 8 || Pin < 0)
  {
    return -1;  //Fail if pin out of range
  }

  if(PinType == INPUT)
  {
    PinModeConf = PinModeConf | (0x01 << Pin);
    // Serial.print("Reg 0x03 = ");  //DEBUG!
    // Serial.println(PinModeConf, HEX); //DEBUG!
    SetDirection(PinModeConf);
    return 1;
  }
  else if(PinType == OUTPUT)
  {
    PinModeConf = PinModeConf & ~(0x01 << Pin);
    // Serial.print("Reg 0x03 = ");  //DEBUG!
    // Serial.println(PinModeConf, HEX); //DEBUG!
    SetDirection(PinModeConf);
    return 0;
  }
  else 
    return -1; //Fail if pin type not defined 
}

int TCA9534::DigitalWrite(int Pin, boolean State)
{
  if(Pin > 8 || Pin < 0)
  {
    return -1; //Fail if pin out of range
  }

  if(State == HIGH)
  {
    Port = Port | (0x01 << Pin);
    // Serial.print("Reg 0x01 = ");  //DEBUG!
    // Serial.println(Port, HEX); //DEBUG!
    SetPort(Port);
    return 1;
  }
  else if(State == LOW)
  {
    Port = Port & ~(0x01 << Pin);
    // Serial.print("Reg 0x01 = ");  //DEBUG!
    // Serial.println(Port, HEX); //DEBUG!
    SetPort(Port);
    return 0;
  }
  else 
    return -1; //Fail if state is ill-defined
}

int TCA9534::SetPort(int Config) {
  Wire.beginTransmission(ADR); // transmit to device with address ADR
  Wire.write(0x01);   //Send to output set register
  Wire.write(Config);   
  return Wire.endTransmission();
}

int TCA9534::SetDirection(int Config) {
  Wire.beginTransmission(ADR); // transmit to device with address ADR
  Wire.write(0x03);        //Send to port configuration register
  Wire.write(Config);              
  return Wire.endTransmission();    // stop transmitting
}

int TCA9534::SetPolarity(int Config) {
  Wire.beginTransmission(ADR); // transmit to device with address ADR
  Wire.write(0x02);        //Send to port configuration register
  Wire.write(Config);              
  return Wire.endTransmission();    // stop transmitting
}

//IN DEVELOPMENT
int TCA9534::ReadPort(int Config) {
  Wire.beginTransmission(ADR); // transmit to device with address ADR
  Wire.write(0x01);        //Send to port configuration register
  Wire.write(Config);              //Set port 1-4 as OUTPUT, all others as inputs
  return Wire.endTransmission();    // stop transmitting
}





