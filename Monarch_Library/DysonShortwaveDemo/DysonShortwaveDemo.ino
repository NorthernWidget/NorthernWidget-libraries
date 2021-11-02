//DysonShortwaveDemo.ino
#include <DysonSW.h>

DysonSW PyroUp;

void setup() {
	pinMode(19, OUTPUT); //DEBUG!
	digitalWrite(19, LOW); //DEBUG!
	PyroUp.begin(UP);
	Serial.begin(38400);
	Serial.println("Welcome to the Dyson Sphere...");
	Serial.println(PyroUp.GetHeader()); //Print data info
}

void loop() {
	// Serial.print("Lux = ");
	Serial.println(PyroUp.GetString());  //Prints out auto-ranged lux values
	delay(1000);
}