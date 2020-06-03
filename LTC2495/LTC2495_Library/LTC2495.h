#ifndef LTC2495_h
#define LTC2495_h

#include "Arduino.h"

class LTC2495
{
  public:
    LTC2495(int _ADR, float _RefVoltage, int _RefPin, float _RefVal);
    LTC2495(); //Over define, assume FIND ADDRESS and assumes 5V referance 
    int Begin(void);

    float GetVoltage(int Pin);
    float GetVoltage(int Pin, int Gain);
    float GetVoltageComp(int Pin);

    float GetVoltageDiff(int Pin1, int Pin2);
    float GetVoltageDiff(int Pin1, int Pin2, int Gain);

    long GetVoltageRaw(int Pin); //Do not scale for ref voltage
    long GetVoltageRaw(int Pin, int Gain);

    long GetVoltageDiffRaw(int Pin1, int Pin2);
    long GetVoltageDiffRaw(int Pin1, int Pin2, int Gain);

    float GetTemp(void); //IMPLEMENT



    char OverFlow;
    char PinError;
    char SignBit;
  private:
  	float RefVoltage;
    int ADR;
    int RefPin;
    float RefVal;
    char GetPin(int Pin); //Calculates 1st byte of I2C request using the pin
    char GetPin(int Pin1, int Pin2); 
    void GetData(char Byte1, char Byte2);
    char ControlByte; //Set control byte for desired values of second control byte
    unsigned int DataA;
    unsigned int DataB;
    unsigned int DataC;

};

#endif