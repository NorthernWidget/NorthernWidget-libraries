#include "Margay.h"
#include <MonarchSW.h>

MonarchSW PyroUp(UP); //Initialzie Upward facing Monarch short wave
MonarchSW	PyroDown(DOWN); //Initialize Downward facing Monarch short wave

String Header = ""; //Information header
uint8_t I2CVals[1] = {0x77}; 
unsigned long UpdateRate = 60; //Number of seconds between readings 

Margay Logger(Model_1v0);

void setup() {
	Header = Header + PyroUp.GetHeader() + PyroDown.GetHeader();
	Logger.begin(I2CVals, sizeof(I2CVals), Header); //Pass header info to logger
	Init();
}

void loop() {
	Logger.Run(Update, UpdateRate);
}

String Update() 
{
	Init();
	return PyroUp.GetString() + PyroDown.GetString();
}

void Init() 
{
	PyroUp.begin();
	PyroDown.begin();
}
