#include <TP_Downhole.h>

TP_Downhole Sensor; //Initalize TP-Downhole sensor

void setup() {
  Serial.begin(9600);  //Begin serial 
  Serial.println("Welcome to the Machine...\n\n");  //Print default dummy message
  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);
  Sensor.begin(TP5v1);  //Select sensor type, see options below
  //Sensor type will be embost in the body of the sensor, and on a label at the top of the cable
  //This type designator identifies the configuration of the sensor address and other features
  //Values include:
  //TP2v2
  //TP2v1
  //TP5v1
  //TP2b2
  //TP2A2

}

void loop() {
  float Pressure = Sensor.getPressure();  //Read pressure from MS5803
  float Temp0 = Sensor.getTemperature(0);  //Read temp from MS5803
  float Temp1 = Sensor.getTemperature(1);  //Read temp from thermistor
  Serial.print("Pressure = ");   //Print pressure read in milli Bars
  Serial.print(Pressure);
  Serial.println(" mBar");
  Serial.print("Temp0 = ");  //Print MS5803 temp in degrees C
  Serial.print(Temp0);
  Serial.print("\tTemp1 = ");  //Print thermistor temp in degrees C
  Serial.println(Temp1);
  Serial.print("\n\n");

  delay(1000);  //Wait a moment to not spam the user 
}
