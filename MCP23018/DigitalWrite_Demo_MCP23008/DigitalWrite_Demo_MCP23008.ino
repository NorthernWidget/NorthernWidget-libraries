/******************************************************************************
DigitalWrite_Demo_MCP23008.ino
Demo to demonstrate the utilization of the interface for the MCP23008
Bobby Schulz @ Northern Widget LLC
10/18/2019
https://github.com/NorthernWidget-Skunkworks/MCP23018

Toggles the states of the pins 0, 1, 2 at variable rates to demonstrate basic use of the digital write functionality. 
Connect LEDs or other indicator to these pins to observe output

0.0.0

"All existing things are really one"
-Zhuangzi

Distributed as-is; no warranty is given.
******************************************************************************/

#include <MCP23008.h>

MCP23008 IO(0x20);
const unsigned long PeriodMain = 1000; //Primary period is 1000ms
const unsigned long PeriodSecondary = 250; //Secondary period is 250ms

void setup() {
	IO.PinMode(0, OUTPUT); //Set pin 0 as output
	IO.PinMode(1, OUTPUT); //Set pin 1 as output
	IO.PinMode(2, OUTPUT); //Set pin 2 as output

	for(int i = 0; i < 5; i++) {  //DEBUG!
		IO.DigitalWrite(0,1);
		delay(100);
		IO.DigitalWrite(0,0);
		delay(100);
	}

}

void loop() {

	static boolean StateMain = false; //Equivalent to off
	static boolean StateSecondary = false; //Equivalent to off

	while(1) {
		if(millis() % PeriodMain == 0) { //If main period rolls over
			StateMain = !StateMain; //Toggle state 
			IO.DigitalWrite(0, int(StateMain)); //Set state value
			IO.DigitalWrite(1, int(!StateMain));  //Set inverse state value
		}
		if(millis() % PeriodSecondary == 0) { //If secondary period rolls over
			StateSecondary = !StateSecondary; //Toggle state 
			IO.DigitalWrite(2, StateSecondary); //Set state value
		}
	}
}