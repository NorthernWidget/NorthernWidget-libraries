#include <MCP3421.h>

MCP3421 adc; //Initialize MCP3425 with default address, 0x6A

void setup() {
  Serial.begin(9600); //Begin serial comunication
  adc.Begin(); //Initialize ADC
  Serial.print("Welcome to the Machine...\nBegin MCP3421 Demonstration\n\n"); //Token greating to verify Serial
}

void loop() {
  adc.SetResolution(12);
  PrintVoltage(adc.GetVoltage(true), 12);  //Read at 12 bit resolution
  adc.SetResolution(14);
  PrintVoltage(adc.GetVoltage(true), 14);  //Read at 14 bit resolution 
  adc.SetResolution(16);
  PrintVoltage(adc.GetVoltage(true), 16);  //Read at 16 bit resolution
  adc.SetResolution(18);
  PrintVoltage(adc.GetVoltage(true), 18);  //Read at 18 bit resolution
  Serial.print("\n\n");
  delay(500); //delay some period between outputs as to not hurt ones eyes with streaming text
}

void PrintVoltage(float Voltage, int Bits) {
  String BitLine = "Voltage, " + String(Bits) + " bit = ";
  Serial.print(BitLine);
  Serial.print(Voltage, Bits - 9); //Print voltage, rounded to the significant digits
  Serial.print(" V\n");
}
