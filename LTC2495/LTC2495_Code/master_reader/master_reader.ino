// Wire Master Reader
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Reads data from an I2C/TWI slave device
// Refer to the "Wire Slave Sender" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>

void setup() {
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
  Serial.println("Welcome to the ADC Machine...");
}


void loop() {

//  for(int x = 0; x<256; x++){
//  Wire.beginTransmission(x);
//  Wire.write(0xBF);
//  Serial.println(Wire.endTransmission());
//  }
 
  Wire.beginTransmission(0x45);
//  Wire.write(0xB0); //Pin 0
//  Wire.write(0xB8); //Pin 1
//  Wire.write(0xB1); //Pin 2
//  Wire.write(0xBF);
//  Wire.write(0x9F); //Pin 14
  Wire.write(0xB4); //Pin 8
  //Wire.write(0xA4); //Pin 8 w/ pin 9 as ref (-in)
  Wire.write(0x80); //Force reset to default
//  Wire.write(0xC0); //Meeasure temp
  Serial.println(Wire.endTransmission());
  //Wire.write(0x00);
  //Serial.println("BANG!");
  //Serial.println(Wire.endTransmission());
  
  delay(200);
  Wire.requestFrom(0x45, 3);    // request 6 bytes from slave device #8
  Serial.println(Wire.available());
  unsigned int ResultA = Wire.read();
  unsigned int ResultB = Wire.read();
  unsigned int ResultC = Wire.read();
//
  Serial.print("Result A = ");
  Serial.println(ResultA);
  Serial.print("Result B = ");
  Serial.println(ResultB);
  Serial.print("Result C = ");
  Serial.println(ResultC);


 signed int VoltageNum = (((ResultA & 0x3F) << 10) | (ResultB << 2) | (ResultC >> 6))/2;
 float Voltage = (float(VoltageNum)/65536.0)*float(3.3);
  Serial.println(VoltageNum);
  Serial.println(Voltage, 8);
  Serial.println("");
//  while (Wire.available()) { // slave may send less than requested
//    char c = Wire.read(); // receive a byte as character
//    Serial.print(c);         // print the character
//  }

  delay(500);
}
