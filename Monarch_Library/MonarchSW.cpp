/******************************************************************************
MonarchSW.cpp
Library for interfacing with the Project Monarch short wave pyranometer 
Bobby Schulz @ Northern Widget LLC
7/5/2018
https://github.com/NorthernWidget-Skunkworks/Project-Monarch

This script is used to interface to the Monarch shortwave pyranometer module and provide 
conversions for some of the values into the appropriate human-readable units.

"The laws of nature are constructed in such a way as to make the universe as interesting as possible."
-Freeman Dyson

Distributed as-is; no warranty is given.
******************************************************************************/

#include "Wire.h"
#include "math.h"
#include "MonarchSW.h"

MonarchSW::MonarchSW(uint8_t Orientation_) 
{
  Orientation = Orientation_; //Copy value to global
  if(Orientation == DOWN) {
    Accel_ADR = 0x53; //Alternate accelerometer address
    ADR = 0x41; //Alternate software address
  }

  else {
    Accel_ADR = 0x1D; //Default accelerometer address
    ADR = 0x40; //Default software address
  }
}

uint8_t MonarchSW::begin() 
{
	Wire.begin();
  InitAccel();
	// Serial.begin(38400); //DEBUG!

}

uint8_t MonarchSW::InitAccel()  //Add variable address ability??
{
  WriteByte(Accel_ADR, 0x2D, 0x08); //Turn on accelerometer
  WriteByte(Accel_ADR, 0x31, 0x08); //
  WriteByte(Accel_ADR, 0x38, 0x00); //Bypass FIFO
  WriteByte(Accel_ADR, 0x2C, 0x0A); //Setup rate
}

float MonarchSW::GetG(uint8_t Axis) 
{
  WriteByte(Accel_ADR, 0x2D, 0x08); //Turn on accelerometer
  // WriteByte(Accel_ADR, XAXIS);
  delay(100); //DEBUG!
  Wire.beginTransmission(Accel_ADR);
  Wire.write(XAXIS + 2*Axis);
  Wire.endTransmission();
  Wire.beginTransmission(Accel_ADR);
  Wire.requestFrom(Accel_ADR, 2);
  int LSB = Wire.read();
  int MSB = Wire.read();
  Wire.endTransmission();
  // return ReadWord(Accel_ADR, XAXIS); 
  // Serial.print(MSB, HEX); Serial.println(LSB, HEX); //DEBUG!
  float g = ((MSB << 8) | LSB)*(0.0039); 
  return g; 
}

float MonarchSW::GetAngle(uint8_t Axis)
{
  float ValX = GetG(0); //Used to get g values
  float ValY = GetG(1);
  float ValZ = GetG(2);
  float Val = 0;
  switch(Axis) {
    case(0):
      Val = asin(ValX); 
      break;
    case(1):
      Val = asin(ValY);
      break;
    case(2):
      Val = acos(ValZ);
      break;
    case(3):
      Val = atan(ValX/(sqrt(pow(ValY, 2) + pow(ValZ, 2))))*(180.0/3.14); //Return pitch angle
      break;
    case(4):
      Val = atan(ValY/(sqrt(pow(ValX, 2) + pow(ValZ, 2))))*(180.0/3.14); //Return roll angle
      break;
  }
  if(ValX == ValY && ValX == ValZ) Val = -9999; //Return error value is all vals are the same (1 in 6.87x10^10 likelyhood of occouring without error)
  return Val; 
}

long MonarchSW::GetUVA() 
{
  long LSW = ReadWord(ADR, UVA_ADR); //Read low word
  long MSW = ReadWord(ADR, UVA_ADR + 2); //Read high word
  return (MSW << 16) | LSW; //Return concatenated result
}

long MonarchSW::GetUVB()
{
  long LSW = ReadWord(ADR, UVB_ADR); //Read low word
  long MSW = ReadWord(ADR, UVB_ADR + 2); //Read high word
  return (MSW << 16) | LSW; //Return concatenated result
}

unsigned int MonarchSW::GetALS()
{
	return ReadWord(ADR, ALS_ADR); //Read back als value
}

unsigned int MonarchSW::GetWhite()
{
  return ReadWord(ADR, WHITE_ADR); //Read back white value
}

float MonarchSW::GetLux()
{
  return float(ReadWord(ADR, ALS_ADR))*float(ReadWord(ADR, LUXMUL_ADR))*LuxRes;  //Multiply lux value by set gain from LuxMul
}

float MonarchSW::GetIR_Short()
{
  return float(ReadWord(ADR, IR_SHORT_ADR))*(1.25e-4);
}

float MonarchSW::GetIR_Mid()
{
  return float(ReadWord(ADR, IR_MID_ADR))*(1.25e-4);
}

float MonarchSW::GetTemp()
{
  float Val = float(ReadWord(ADR, THERM_ADR))*(1.25e-4);
  return TempConvert(Val, 3.3, 10000.0, A, B, C, D, 10000.0) - 273.15;
}

float MonarchSW::TempConvert(float V, float Vcc, float R, float A, float B, float C, float D, float R25)
{
  float Rt = (Vcc/V)*R - R;  //Use if thermistor is on TOP side of voltage divider
  // float Rt = -R/(1 - (Vcc/V)); //Use if thermistor is on BOTTOM side of voltage divider  
  float LogRt = log(Rt/R25);
  float T = 1.0/(A + B*LogRt + C*pow(LogRt, 2.0) + D*pow(LogRt, 3.0));
  return T;
}

String MonarchSW::GetString()
{
	String Data = "";
	Data = String(GetAngle(3)) + "," + String(GetAngle(4)) + "," + String(GetUVA()) + "," + String(GetUVB()) + "," + String(GetWhite()) + "," + String(GetLux()) + "," + String(GetIR_Short()) + "," + String(GetIR_Mid()) + "," + String(GetTemp()) + ",";
	return Data;
}

String MonarchSW::GetHeader()
{
	String Header = "";
	if(Orientation == UP) Header = "R_u [deg], P_u [deg], UVA_u, UVB_u, White_u, Vis_u [lx], IR_S_u, IR_M_u, PyroT_u [C], ";
	if(Orientation == DOWN) Header = "R_d [deg], P_d [deg], UVA_d, UVB_d, White_d, Vis_d [lx], IR_S_d, IR_M_d, PyroT_d [C], ";
	return Header;
}

void MonarchSW::PrintAllRegs() 
{
  for(int i = 0; i < 0x1A; i++) {
    Serial.print("Reg"); Serial.print(i, HEX); Serial.print(":\t"); //Print register number
    Serial.println(ReadByte(ADR, i)); //Print register value
  }
  Serial.print("\n\n");
}

uint8_t MonarchSW::WriteByte(uint8_t Adr, uint8_t Pos, uint8_t Val)
{
  Wire.beginTransmission(Adr);
  Wire.write(Pos);  //Identify register
  Wire.write(Val);  //Write desired value to register
  Wire.endTransmission(); //End I2C message
}

// uint8_t WriteByte(uint8_t Adr, uint8_t Val)
// {
//   Wire.beginTransmission(Adr);
//   Wire.write(Val);  //Write desired value to register
//   Wire.endTransmission(); //End I2C message
// }

uint8_t MonarchSW::ReadByte(uint8_t Adr, uint8_t Pos)
{
  Wire.beginTransmission(Adr);
  Wire.write(Pos);  //Read from desired position
  Wire.endTransmission(true); 
  Wire.requestFrom(Adr, 1); //Read a single byte
  // while(Wire.available() < 1); //Wait for byte to be read in
  return Wire.read(); //Read the desired value back
}

unsigned int MonarchSW::ReadWord(uint8_t Adr, uint8_t Pos)
{
  unsigned int LSB = ReadByte(Adr, Pos);
  unsigned int MSB = ReadByte(Adr, Pos + 1);
  return (MSB << 8) | LSB; //Read the desired value back
}
