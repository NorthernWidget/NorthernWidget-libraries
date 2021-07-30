#include <LTC2495.h>
#include <Wire.h>

LTC2495 ADC1(0x45, 3.3);
void setup() {
  while (!SerialUSB);
  SerialUSB.begin(9600);
  SerialUSB.println("Welcome to the ADC Machine...");
  SerialUSB.print("ADC Status ");
  SerialUSB.println(ADC1.Begin());
}

void loop() {
  // put your main code here, to run repeatedly:
  float Voltage = ADC1.GetVoltage(8);
  float Temp = ADC1.GetTemp();
  //Serial.print("Volatge = "); Serial.println(Voltage);
  SerialUSB.print("Temp = "); SerialUSB.print(Temp); SerialUSB.println("C");

  for(int i = 0; i < 16; i++) {
    SerialUSB.print("Voltage "); SerialUSB.print(i); SerialUSB.print(" = ");
    SerialUSB.println(ADC1.GetVoltage(i));
  }
  delay(1);
}
