#include "Haar.h"

Haar::Haar()
{
}

uint8_t Haar::begin(uint8_t ADR_)
{
	ADR = ADR_;
	Wire.begin();
	updateMeasurements();
}

float Haar::getPressure(bool update) //Get pressure in mBar
{
	if(update) updateMeasurements(); //Only call for updated value if requested
	uint32_t val = 0; //Val for getting/calculating pressure value
	uint32_t Temp = 0; //DEBUG

	for(int i = 0; i < 3; i++) {
		Wire.beginTransmission(ADR);
		Wire.write(PRES_REG + i);
		Wire.endTransmission();
		Wire.requestFrom(ADR, 1);
		Temp = Wire.read(); //DEBUG!
		val = (Temp << 8*i) | val; //DEBUG!
	}
	dataRequested = false; //Clear flag on data retreval
  	return (val / 4096.0);
}

float Haar::getHumidity(bool update)  //Return humidity in % (realtive)
{
	if(update) updateMeasurements(); //Only call for updated value if requested
	float val = 0; //Val for getting/calculating RH value
	val = (uint16_t(getWord(RH_REG)));
	val = (100.0*val)/65535.0;  //Convert to RH
	dataRequested = false; //Clear flag on data retreval
	return val;
}

float Haar::getTemperature(Sensor device, bool update)  //Return temp in C
{
	if(update) updateMeasurements(); //Only call for updated value if requested
	float val = 0; //Val for getting/calculating temp value
	if(device == Pres_Sense) {
		val = getWord(TEMP_PRES);
		val = val/100.0; //Convert to C
	}

	else if (device == RH_Sense) {
		val = getWord(TEMP_RH);
		val = ((val*175.0)/65535.0) - 45; //Convert to C
	}
	dataRequested = false; // Clear flag on data retreval
	return val;
}

uint8_t Haar::sleep(bool state)
{
	// Add sleep function!
}

// FIX! Allow for read of status register in order to not overwrite state bits
uint8_t Haar::updateMeasurements(bool block)
{
	dataRequested = true; // Set flag
	Wire.beginTransmission(ADR);
	Wire.write(0x00);
	Wire.write(0x01); // Trigger conversion
	uint8_t error = Wire.endTransmission(); // Return I2C status
	// Only block if triggered
	if(block) {
		// Get timeout value
		unsigned long timeout = millis();
		// Wait for new data to be returned
		while(!newData() && (millis() - timeout < timeoutGlobal)) {
			delay(1);
		}
		return error;
	}
	else return error;
}

String Haar::getHeader()
{
	return "Pressure Atmos [mBar], Humidity [%], Temp Pres [C], Temp RH [C],";
}

String Haar::getString()
{
	if(dataRequested) {  //If new data is already en-route
		unsigned long timeout = millis(); //Get timeout value
		//Wait for new data to be returned
		while(!newData() && (millis() - timeout < timeoutGlobal)) {
			delay(1);
		}
	}
	else(updateMeasurements(true)); //Else, block for new conversion
	// delay(100); //DEBUG!
	return String(getPressure()) + "," + String(getHumidity()) + "," \
					+ String(getTemperature(Pres_Sense)) + "," \
					+ String(getTemperature(RH_Sense)) + ",";
}

bool Haar::newData()  // Checks for updated data
{
	unsigned long timeout = millis(); // Get timeout value
	Wire.beginTransmission(ADR);
	Wire.write(0x00);
	Wire.endTransmission();
	Wire.requestFrom(ADR, 1);
	// Wait for value to be returned //FIX! add timeout/remove
	while(Wire.available() < 1 && (millis() - timeout < timeoutGlobal)) {
		delay(1);
	}
	uint8_t val = Wire.read();  //DEBUG!
	bool state = false;
	// bool state = ~(val & 0x01);
	if(val & 0x01 == 1) state = false;  //FIX! Make cleaner
	else state = true;
	// Serial.println(state); //DEBUG!
	// Return inverse of bit 0, true when bit has been cleared,
	// false when waiting for new conversion
	return (state);
}

int16_t Haar::getWord(uint8_t Reg)  //Returns word, read from Reg position
{
	uint16_t val = 0; //Val to be read from device
	Wire.beginTransmission(ADR);
	Wire.write(Reg);
	Wire.endTransmission();

	Wire.requestFrom(ADR, 1);  // Request word
	//while(Wire.available() < 2); //Wait //FIX! Add timeout
	val = Wire.read();
	// Serial.println(val, HEX);

  Wire.beginTransmission(ADR);
  Wire.write(Reg + 1);
  Wire.endTransmission();
  Wire.requestFrom(ADR, 1);  //Request word
	val = val | (Wire.read() << 8);  //Concatonate 16 bits
	//val = Wire.read() | (val << 8);  //Concatonate 16 bits //DEBUG!
	return val;
}
