/******************************************************************************
TP_Downhole_Longbow.h
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

#ifndef TP_Downhole_Longbow_h
#define TP_Downhole_Longbow_h

#include <Arduino.h>

class TP_Downhole_Longbow
{
public:
    TP_Downhole_Longbow();  // Constructor
    uint8_t begin(uint8_t _ADR); //Begin over UART
    uint8_t begin(uint8_t _ADR, uint8_t _WidgetADR); //Begin over I2C using widget

    // Return calculated temperature from sensor
    float getTemperature(uint8_t Location);
    float getTemperature();
    // Return calculated pressure from sensor
    float getPressure();
    String GetHeader();
    String GetString();
    //Longbow utilities
    uint8_t GetBaud();
    uint8_t SetBaud(long Baud);
    uint8_t SetAddress(uint8_t NewAdr, uint8_t Adr); 
    uint8_t SetWidgetAddress(uint8_t NewAdr);
    uint8_t Reset();
    uint8_t GetPacket(uint8_t Reg, uint8_t Adr);
    char GetByte(uint8_t RegID);
    void GetData(char *Data, uint8_t RegID, uint8_t NumBytes);

private:
    uint8_t Control = 0; //Control byte
    uint8_t ADR_Out = 0; //Output address (from slave)
    uint8_t RegID_Out = 0; //Output register ID (from slave)
    float Data_Out = 0; //Output data (From slave)
    char Format_Out = 0; //Output data fromatter (from slave)
    uint8_t CRC_Out = 0; //Output crc check (from slave)

    uint8_t ADR; //Longbow address (RS485)
    uint8_t WidgetADR; //Widget address (I2C)

    //Data manipulation
    uint8_t CharToInt(char *Data);
    void IntToArray(char *Reg, uint8_t Data);
    //I2C utilities 
    void SendData(uint8_t Reg, char *Data, uint8_t Len);
    void SendData(uint8_t Reg, uint8_t Data);
};

#endif
