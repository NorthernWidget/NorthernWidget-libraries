/******************************************************************************
MCP3421.cpp
Library for MCP3421 ADC
Bobby Schulz @ Northern Widget LLC & UMN
2/20/2018

MCP3421 is a 18 bit ADC, PGA, and various modes of resolution operation in order to facilitate various data rates

"Simplicity is prerquisite for reliability"
-Edsger W. Dijkstra

Distributed as-is; no warranty is given.
******************************************************************************/

#include <Arduino.h>
#include <MCP3421.h>
#include <Wire.h>


MCP3421::MCP3421(int _ADR)
{
  ADR = _ADR; 
  #if defined(ARDUINO) && ARDUINO >= 100 
    Wire.begin();
  #elif defined(PARTICLE)
    if(!Wire.isEnabled()) Wire.begin(); //Only initialize I2C if not done already //INCLUDE FOR USE WITH PARTICLE 
  #endif
}

MCP3421::MCP3421(void)  
{
  ADR = 0x6A; //Use 0x6A as Address by default  
  #if defined(ARDUINO) && ARDUINO >= 100 
    Wire.begin();
  #elif defined(PARTICLE)
    if(!Wire.isEnabled()) Wire.begin(); //Only initialize I2C if not done already //INCLUDE FOR USE WITH PARTICLE 
  #endif
}

int MCP3421::Begin(int _ADR) //Initialize the system in 1x gain, with 12 bit resolution, continuious conversions
{
  ADR = _ADR; //Allow for ADR to be set in begin statment as well
  return Begin();
}

int MCP3421::Begin(void) //Initialize the system in 1x gain, with 12 bit resolution, continuious conversions
{
  SetGain(1);
  SetResolution(12); // measurement duration defaults to that required for 12 bit
  SetMode(CONTINUIOUS);
  Wire.beginTransmission(ADR);
  return Wire.endTransmission(); //Return I2C status 
}

//Returns the bit value from the conversion, from 0 to 2^n
long MCP3421::GetVoltageRaw(bool WaitForVal) {
  int Data[4];
  
  int Config = GetConfig(); //Get config register to test if single shot or continuios operation is being used
  if((Config & 0x10) == SINGLE_SHOT) { //If single shot is being used, intiate a new conversion
    Config = Config | 0x80;
    Wire.beginTransmission(ADR);
    Wire.write(Config); //Set conversion bit
    Wire.endTransmission();
    delay(1);

  }

    while((Config & 0x80) != 0 && WaitForVal == true) { //Wait for next conversion (for both single shot or continuious), only if told to wait for new value
      Config = GetConfig(); //Test register for new value to be read 
    }

  delay(measurement_duration_ms); // Wait the requisite amount of time to take a measurement

  Wire.requestFrom(ADR, 4);
  
  if(Wire.available() == 4) //Get data bytes, 3 bytes of potential data and configuration register 
  {
    Data[0] = Wire.read();
    Data[1] = Wire.read();
    Data[2] = Wire.read();
    Data[3] = Wire.read();
  }

  int NumBits = 12 + ((Data[3] & 0x0C) >> 2)*2; //Calulate the number of bits used for conversion from config register

  unsigned long RawADC = 0; //Use appropriate conversion math based on the number of bits used in the conversion
  if(NumBits == 12) RawADC = ((Data[0] & 0x0F) << 8) + Data[1];
  if(NumBits == 14) RawADC = ((Data[0] & 0x3F) << 8) + Data[1];
  if(NumBits == 16) RawADC = ((Data[0] & 0xFF) << 8) + Data[1];
  if(NumBits == 18) RawADC = ((long(Data[0]) & 0x03) << 16) + (long(Data[1]) << 8) + Data[2];
    
  if(RawADC > pow(2, NumBits)/2 - 1) //REMOVE??
  {
    RawADC -= pow(2, NumBits) - 1;
  }
  
  Config = GetConfig();  //Get congiuration register to do bit masking with
  Wire.beginTransmission(ADR);
  Wire.write(Config & 0xEF); //Clear conversion bit to ensure result is not reread 
  Wire.endTransmission();

  return RawADC;  //return raw result
}

//Returns float voltage value in volts
float MCP3421::GetVoltage(bool WaitForVal) {
  long Raw = GetVoltageRaw(WaitForVal); //Get the raw bits, pass on stale value flag
  int Config = GetConfig(); //Get configuration register 

  int Gain = round(pow(2, (Config & 0x03))); //Calc gain from config register 
  int NumBits = 12 + ((Config & 0x0C) >> 2)*2; //Calc number of bits from config register 
  return (float(Raw)*4.096)/float(round(pow(2, NumBits)));  //Return converted value 
//  return (float(Raw)*(4.096/pow(2, NumBits)))/(float(Gain));
}

//Sets the gain of the internal PGA, with either 1x, 2x, 4x, or 8x
//Takes as input the desired gain factor 
//Returns result of I2C comunication if gain value is valid, or returns -1 if gain value is not valid
int MCP3421::SetGain(int DesiredGain) {
  boolean ValidGain = false;
  for(int i = 0; i < 4; i++){ //Test if gain value is valid
    if(round(pow(2, i)) == DesiredGain) ValidGain = true;
  }

  if(ValidGain){ //If gain value is valid, attempt to set new gain value
    int Config = GetConfig();
    Config = (Config & 0xFC) | int(log(DesiredGain)/log(2));
    Wire.beginTransmission(ADR);
    Wire.write(Config);
    return Wire.endTransmission(); //Return results of attempt
  }

  else return -1; //If gain value is not valid, return -1
}

//Sets the resultion of the converter, either 12 bit, 14 bit, 16 bit, or 18 bit
//Takes as input the desired resolution in number of bits 
//Returns the result of the I2C comunication if the resolution is valid, or returns -1 if resolution value is not valid 
int MCP3421::SetResolution(int DesiredResolution) {
  boolean ValidResolution = false;
  for(int i = 0; i < 4; i++){ //Test if resolution value is valid
    if(12 + 2*i == DesiredResolution) ValidResolution = true;
  }

  // Set measurement duration based on resolution  
  if(DesiredResolution == 12){
    measurement_duration_ms = 5;
  }
  else if(DesiredResolution == 14){
    measurement_duration_ms = 17;
  }
  else if(DesiredResolution == 16){
    measurement_duration_ms = 67;
  }
  else if(DesiredResolution == 18){
    measurement_duration_ms = 267;
  }

  if(ValidResolution){ //If resolution value is valid, attempt to set new resolution
    int Config = GetConfig();
    Config = (Config & 0xF3) | ((DesiredResolution - 12)/2) << 2;
    Wire.beginTransmission(ADR);
    Wire.write(Config);
    return Wire.endTransmission();  //return result of attempt
  }

  else return -1; //If resolution value is not valid, return -1
}

//Returns the configuration register of the device, used for testing status of conversion, mode of operation, gain and resolution settings
int MCP3421::GetConfig() {
  int Data[4];
    
  Wire.requestFrom(ADR, 4);
  if(Wire.available() == 4)  //Get 4 bytes from device
  {
    Data[0] = Wire.read();
    Data[1] = Wire.read();
    Data[2] = Wire.read();
    Data[3] = Wire.read();
  }

  return Data[3]; //Return only last byte, configuration register 
}


//Sets the mode of operation, either continuious or single shot operation
//Takes as input the desired mode, either CONTINUIOUS or SINGLE_SHOT
//Returns either the result of the attempt if input is valid, or -1 if input is not valid (neither CONTINUIOUS or SINGLE_SHOT)
int MCP3421::SetMode(int Mode) {
  int Config  = GetConfig();  //Get current configuration register 
  if(Mode % 2 == 0) { //Value is either 0 or 1 and therefore valid, proceed 
    Config = (Config & 0xEF) | (Mode << 4); //Generate bit masked configuration register in order to set/clear mode bit
    Wire.beginTransmission(ADR);  //Write new configuration register  
    Wire.write(Config); 
    return Wire.endTransmission(); //Return result of attempt
  }
  else return -1; //If not valid input, return -1 
}





