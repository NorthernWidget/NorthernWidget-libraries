#include <Haar.h>


Haar sensor; //Instantiate Haar sensor
unsigned long period = 5000; //Number of ms to wait between sensor readings

void setup() {

	Serial.begin(115200); //Begin serial conunication at high speed 
	uint8_t error = sensor.begin(); //Initialize Haar sensor with default sensor, grab sensor status
	
	Serial.print("Haar Status:"); //Print resulting sensor status 
	if(error != 0) Serial.print("\t FAIL!\n\n");
	else Serial.print("\t PASS\n\n");
}

void loop() {
	sensor.updateMeasurements(true); //Update data, block until process is complete
	float humidity = sensor.getHumidity(); //Grab new humidity values [%]
	float pressure = sensor.getPressure(); //Grab new pressure values [mBar]
	float temp = sensor.getTemperature(); //Grab new temp values [°C]

	Serial.print("Temp: "); Serial.print(temp, 4); Serial.print(" °C\t"); //Print temp to 4 decimal places
	Serial.print("Pressure: "); Serial.print(pressure, 2); Serial.print(" mBar\t"); //Print pressure to 2 decimal places
	Serial.print("Humidity: "); Serial.print(humidity, 2); Serial.print(" %\n"); //Print humidity to 2 decimal places
	delay(period); //Wait a specified period before the next sensor reading
}