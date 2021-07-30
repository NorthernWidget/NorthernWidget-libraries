#include <Arduino.h>
#include <LTC2495.h>
#include <Wire.h>


LTC2495::LTC2495(int _ADR, float _RefVoltage, int _RefPin, float _RefVal)
{
  ADR = _ADR; 
  RefVoltage = _RefVoltage; //Define global ref voltage 
  RefPin = _RefPin;
  RefVal = _RefVal;
  Wire.begin();  
}

LTC2495::LTC2495(void)  //Overdefine definition for floating CAx pins and 5v single ended referance 
{
  ADR = 0x45; 
  RefVoltage = 5.0; //Define global ref voltage 
  Wire.begin();  
}


int LTC2495::Begin(void)
{
  Wire.beginTransmission(ADR);
  Wire.write(0xA0); //Default POR port to read, use read to detect presance of chip at address 
  Wire.write(0x80); //Force default operation mode 
  if(Wire.endTransmission() != 0) return -1;
  else return 1;
}

float LTC2495::GetVoltage(int Pin)
{
  char Byte1 = GetPin(Pin);
  char Byte2 = 0x80;

  GetData(Byte1, Byte2);

  OverFlow = (DataA >> 7) & ((DataA >> 6) & 0x01); //Set overflow bit if out of FS range NOTE: IMPROVE SETTING OF BIT

  signed int VoltageNum = (((DataA & 0x3F) << 10) | (DataB << 2) | (DataC >> 6))/2; //Get raw ADC result

  if(OverFlow) return RefVoltage/2.0;
  else{
  float Voltage = (float(VoltageNum)/65536.0)*float(RefVoltage);  //Calculate actual voltage 
  return Voltage; 
  }
}

float LTC2495::GetVoltageComp(int Pin)
{
  char Byte1 = GetPin(Pin);
  char Byte2 = 0x80;
  GetData(Byte1, Byte2);

  OverFlow = (DataA >> 7) & ((DataA >> 6) & 0x01); //Set overflow bit if out of FS range NOTE: IMPROVE SETTING OF BIT
  SignBit = (DataA >> 7) & 0x01;
  signed int VoltageNum = (((DataA & 0x3F) << 10) | (DataB << 2) | (DataC >> 6))/2; //Get raw ADC result

  float CompVal = GetVoltage(RefPin);
  float RefComp = RefVal/CompVal;
  float Voltage = 0;

  if(OverFlow) return RefVoltage/2.0;
  else if(SignBit) {

    Voltage = ((float(VoltageNum)/65536.0)*float(RefVoltage) + RefVoltage/2)*RefComp;
    return Voltage;
  }
  else{
    Voltage = (float(VoltageNum)/65536.0)*float(RefVoltage)*RefComp;  //Calculate actual voltage 
    return Voltage;
  }
}

float LTC2495::GetVoltageDiff(int Pin1, int Pin2)
{
  char Byte1 = GetPin(Pin1, Pin2);
  char Byte2 = 0x80;
  float Voltage = 0;

  GetData(Byte1, Byte2);

  OverFlow = (DataA >> 7) & ((DataA >> 6) & 0x01); //Set overflow bit if out of FS range NOTE: IMPROVE SETTING OF BIT
  SignBit = (DataA >> 7);

  signed int VoltageNum = (((DataA & 0x3F) << 10) | (DataB << 2) | (DataC >> 6))/2; //Get raw ADC result

  if(!SignBit){
    Voltage = (float(VoltageNum)/65536.0)*float(RefVoltage) - RefVoltage/2.0;  //Calculate actual voltage 
  }
  else{
    Voltage = (float(VoltageNum)/65536.0)*float(RefVoltage);  //Calculate actual voltage 
  }
  return Voltage; 
}

long LTC2495::GetVoltageRaw(int Pin)
{
  char Byte1 = GetPin(Pin);
  //Serial.print("BANG!");
  //char Byte1 = 0xB4; //DEBUG!
  char Byte2 = 0x80;

  GetData(Byte1, Byte2);

  OverFlow = (DataA >> 7) & ((DataA >> 6) & 0x01); //Set overflow bit if out of FS range NOTE: IMPROVE SETTING OF BIT

  signed int VoltageNum = (((DataA & 0x3F) << 10) | (DataB << 2) | (DataC >> 6))/2; //Get raw ADC result
  return VoltageNum;
}

long LTC2495::GetVoltageDiffRaw(int Pin1, int Pin2)
{
  char Byte1 = GetPin(Pin1, Pin2);
  char Byte2 = 0x80;

  GetData(Byte1, Byte2);

  OverFlow = (DataA >> 7) & ((DataA >> 6) & 0x01); //Set overflow bit if out of FS range NOTE: IMPROVE SETTING OF BIT

  signed int VoltageNum = (((DataA & 0x3F) << 10) | (DataB << 2) | (DataC >> 6))/2; //Get raw ADC result
  return VoltageNum;
}

float LTC2495::GetTemp(void)
{
  char Byte1 = 0xA0;
  char Byte2 = 0xC0;

  GetData(Byte1, Byte2);
  unsigned int TempNum = (((DataA & 0x3F) << 10) | (DataB << 2) | (DataC >> 6));
  float TempK = float(TempNum)*RefVoltage/12.25;
  Serial.println(TempK); //DEBUG!
  float TempC = TempK - float(273.15);
  return TempC;
}

char LTC2495::GetPin(int Pin)
{
  if(Pin < 0 || Pin > 15){
    Serial.println("PIN ERROR"); //DEBUG!
    return 0; //Pin range error THROW EXCEPTION
  }

  char Byte1 = 0xB0; //Set SGL since using single ended measure 
  Byte1 = Byte1 | (Pin >> 1); //Set A0-A2
  Byte1 = Byte1 | ((Pin % 2) << 3); //Set ODD/Sign bit using mod to detect if pin# is even or odd
  return Byte1;
}

char LTC2495::GetPin(int Pin1, int Pin2) //NOTE: BY CONVENTION, PIN1 IS POS!
{
  if(abs(Pin1 - Pin2) != 1) {
    return 0; //Pin range error, THROW EXCEPTION
  }
  char Byte1 = 0xA0; //Set SGL since using single ended measure 
  Byte1 = Byte1 | (min(Pin1, Pin2) >> 1); //Set A0-A2
  Byte1 = Byte1 | ((Pin1 > Pin2) << 3); //Set ODD/Sign bit detecting which pin is pos and neg
  return Byte1;
}

void LTC2495::GetData(char Byte1, char Byte2) 
{
  Wire.beginTransmission(ADR);
  Wire.write(Byte1); //Send desired pin value
  Wire.write(Byte2); //Force reset to default
  Wire.endTransmission();

  Wire.beginTransmission(ADR);
  Wire.write(0x80); //Send desired pin value
  Wire.write(0x00); //Force reset to default
  Wire.endTransmission();


  delay(150);

  Wire.requestFrom(ADR, 3);    // request 6 bytes from slave device #8

  DataA = Wire.read();
  DataB = Wire.read();
  DataC = Wire.read();

  delay(150);
  // Serial.println(DataA); //DEBUG!
  // Serial.println(DataB); //DEBUG!
  // Serial.println(DataC); //DEBUG!
}





