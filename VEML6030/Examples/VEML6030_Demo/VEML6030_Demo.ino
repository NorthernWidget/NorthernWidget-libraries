#include <VEML6030.h>

VEML6030 ALS;

void setup() {
  Serial.begin(38400);  //Begin Serial
  ALS.begin();  //Begin the UV module

}

void loop() {
  ALS.AutoRange(); //Determine the best gain and integration time values for given light intensity
  delay(1000); //Wait for new value to be taken, up to 1 second
  Serial.print("Lux = ");
  Serial.print(ALS.GetLux());  //Get compensated UVA value
  Serial.print(" White = ");
  Serial.print(ALS.GetWhite());  //Get compensated UVB value
  Serial.print(" Ambient Raw = ");
  Serial.println(ALS.GetALS());
  delay(1000);
}
