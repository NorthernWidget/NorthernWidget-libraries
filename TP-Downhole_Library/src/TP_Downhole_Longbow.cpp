/******************************************************************************
TP_Downhole_Longbow.cpp
Library for TP-Downhole Longbow pressure and tempurature sensor, made by Northern Widget LLC.
Bobby Schulz @ Northern Widget LLC
10/10/2018
Hardware info located at: https://github.com/NorthernWidget/TP-DownHole
                          and https://github.com/NorthernWidget-Skunkworks/Project-Longbow

The TP-Downhole Longbow is a small form factor tempreture and pressure sensor which has the same
accuracy and capability of the conventional TP-Downhole, but with an RS-485 comunication system to 
allow for very long cable lengths

"Size matters not. Look at me. Judge me by my size do you? And well you should not. For my ally is the
Force, and a powerful ally it is"
-Yoda

Distributed as-is; no warranty is given.
******************************************************************************/

#include <Wire.h> // Wire library is used for I2C
#include "TP_Downhole_Longbow.h"

TP_Downhole_Longbow::TP_Downhole_Longbow()
{
}

uint8_t TP_Downhole_Longbow::begin(uint8_t _ADR)  //Initialze Longbow using UART
{
    //ADD FUNCTIONALITY!
    // Serial1.begin(Baud); //FIX! figure out how to specify serial connection
    return -1; //Retun failure is both devices are not connected 
}

uint8_t TP_Downhole_Longbow::begin(uint8_t _ADR, uint8_t _WidgetADR)  //Initialzie Longbow using widget over I2C
{
    ADR = _ADR; //Copy to global
    WidgetADR = _WidgetADR;

    Wire.begin();  //Begin I2C
    Wire.beginTransmission(WidgetADR);
    return Wire.endTransmission(); //Retun status of I2C connection
}

float TP_Downhole_Longbow::getTemperature(uint8_t Location) //Returns temp in C from either subsensor
{
    GetPacket(5, ADR); //Call for updated data
    GetPacket(2 - Location, ADR); //Get appropriate data 
    return Data_Out; 
}


float TP_Downhole_Longbow::getTemperature() //By default get thermistor temp value
{
    return getTemperature(1); 
}


float TP_Downhole_Longbow::getPressure() // Return a pressure reading units mBar.
{
    GetPacket(5, ADR); //Call for updated data
    GetPacket(0, ADR); //Get pressure data 
    return Data_Out;
}

String TP_Downhole_Longbow::GetHeader() 
{
    return "Pressure [mBar], Temp DH [C], Temp DHt [C], "; //return header string
}

String TP_Downhole_Longbow::GetString()
{
    return String(getPressure()) + "," + String(getTemperature(0)) + "," + String(getTemperature(1)) + ","; //Return data string
}

uint8_t TP_Downhole_Longbow::SetAddress(uint8_t NewAdr, uint8_t Adr) //Set new Longbow address
{
    char ADR_Temp[2] = {0};
    char ADR_New[2] = {0};
    char AddressReg[2] = {'9', '9'}; 
    IntToArray(ADR_Temp, Adr); //Convert Adr to char array
    IntToArray(ADR_New, NewAdr); //Convert new address to char array

    SendData(0, 1);  //Turn on UART
    SendData(1, ADR_Temp, 2);  //Load I2C register 
    SendData(2, AddressReg, 2);  //Load downhole register
    SendData(3, ADR_New, 2);  //Load data
    SendData(0, 3); //Set send bit, keep UART on
}

uint8_t TP_Downhole_Longbow::SetBaud(long Baud)
{
    Baud = Baud/1200; //Convert to baud multiple
    SendData(97, Baud);
    return GetBaud(); //Read baud back
}

uint8_t TP_Downhole_Longbow::GetBaud()
{
    // GetPacket(5, ADR); //Call for updated data
    GetPacket(96, ADR); //Get baud rate
    return Data_Out;
}

uint8_t TP_Downhole_Longbow::SetWidgetAddress(uint8_t NewAdr)
{
    Wire.beginTransmission(0x00);  //Use general call address
    Wire.write(99);  //Write to address set register
    Wire.write(NewAdr);  //Write new address
    return Wire.endTransmission(); //DEBUG!
}

uint8_t TP_Downhole_Longbow::Reset() 
{
  SendData(0, 1); //Turn on UART
  SendData(0, 0x81); //Keep UART on, command a reset
  delay(100);
}

uint8_t TP_Downhole_Longbow::GetPacket(uint8_t Reg, uint8_t Adr)
{
    char ADR_Temp[2] = {0};
    char Reg_Temp[2] = {0};
    IntToArray(ADR_Temp, Adr); //Convert Adr to char array
    IntToArray(Reg_Temp, Reg); //Convert Reg to char array

    SendData(0, 1); //Turn on UART
    SendData(1, ADR_Temp, 2);  //Send address of device
    SendData(2, Reg_Temp, 2);  //Send desired register 
    SendData(0, 3); //Set send bit, keep UART on

    char DataTemp[10] = {0};
    char CRC_Temp[4] = {0};

    delay(100); //Wait for Longbow Widget to populate the registers, FIX! Read command register and wait instead 
    
    GetData(DataTemp, 0, 1);  //Call for control data
    Control = DataTemp[0];  

    GetData(DataTemp, 1, 2);  //Call for Address data
    ADR_Out = CharToInt(DataTemp);  //Convert to int

    GetData(DataTemp, 2, 2);  //Call for Register ID 
    RegID_Out = CharToInt(DataTemp);  //Convert to int
    
    GetData(DataTemp, 3, 10);  //Call for raw data
    Data_Out = strtod(DataTemp, NULL);  //Convert to float 

    Format_Out = GetByte(4); //Read format character

    GetData(CRC_Temp, 5, 3);  //Call for CRC
    CRC_Out = strtol(CRC_Temp, NULL, 10);  //Convert to int
}

char TP_Downhole_Longbow::GetByte(uint8_t RegID)  //Read single byte of data
{
    Wire.beginTransmission(WidgetADR);  //Send register value
    Wire.write(RegID);
    Wire.endTransmission();

    Wire.requestFrom(WidgetADR, 1);  //Read from register
     while(Wire.available() < 1); 
    return Wire.read();
}

void TP_Downhole_Longbow::GetData(char *Data, uint8_t RegID, uint8_t NumBytes)  //Call for an array of data
{
    Wire.beginTransmission(WidgetADR);  //Send register value
    Wire.write(RegID);
    Wire.endTransmission();

    Wire.requestFrom(WidgetADR, NumBytes);  //Read multiple bytes from register
    int i = 0;
    for(i = 0; i < NumBytes; i++) {
        Data[i] = Wire.read();
    }
    Data[i + 1]  = '\0';  //Add null terminator 
}

uint8_t TP_Downhole_Longbow::CharToInt(char *Data) //Convert 2 char bytes (0 ~ 99) to integer
{
    return (Data[0] - 48)*10 + (Data[1] - 48);
}

void TP_Downhole_Longbow::IntToArray(char *Reg, uint8_t Data)  //Convert Data value (0 ~ 99) to Char array
{
    Reg[1] = (Data % 10) + 48;
    Reg[0] = ((Data - (Data % 10))/10) + 48;
}

void TP_Downhole_Longbow::SendData(uint8_t Reg, char *Data, uint8_t Len) //Writes out array to given register on reciver
{
  Wire.beginTransmission(WidgetADR);
  Wire.write(Reg);
  for(int i = 0; i < Len; i++) {  //Write multiple bytes 
    Wire.write(Data[i]);  
  }
  Wire.endTransmission(); 
}

void TP_Downhole_Longbow::SendData(uint8_t Reg, uint8_t Data) //Writes out single byte to register on reciver 
{
  Wire.beginTransmission(WidgetADR);
  Wire.write(Reg);
  Wire.write(Data);
  Wire.endTransmission();
}