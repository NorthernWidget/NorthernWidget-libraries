#include <LTC2495.h>
#include <Wire.h>

LTC2495 ADC1(0x45, 5.0);
void setup() {
  Serial.begin(9600);
  Serial.println("Welcome to the ADC Machine...");
  Serial.print("ADC Status ");
  Serial.println(ADC1.Begin());
}

void loop() {
  // put your main code here, to run repeatedly:
  float Voltage = ADC1.GetVoltage(8);
  float Temp = ADC1.GetTemp();
  //Serial.print("Volatge = "); Serial.println(Voltage);
//  Serial.print("Temp = "); Serial.print(Temp); Serial.println("C");

  for(int i = 0; i < 16; i++) {
    Serial.print("Voltage "); Serial.print(i); Serial.print(" = ");
    Serial.println(ADC1.GetVoltage(i));
    Serial.println(OverFlow);
    delay(100);
  }
  delay(1);
}
