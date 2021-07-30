/*
Margay Library
Licensed: GNU GPL v3

Written by:
Bobby Schulz
Andy Wickert
*/

#include <Margay.h>
#include <Arduino.h>

// #include <PCINT.h>



// extern void AttachPCI(byte Pin, void (*UserFunc)(void), int Mode);
// extern void enableInterrupt(uint8_t interruptDesignator, void (*userFunction)(void), uint8_t mode);
// uint8_t LogInt = 28; //DEBUG!

// *digitalPinToPCMSK(LogInt) |= bit (digitalPinToPCMSKbit(LogInt));  // enable pin



volatile bool manualLog = false; // Global for interrupt access

volatile uint8_t ExtIntPin = 255; // Sets external interrupt number; 255 for none
String ext_int_header_entry;
volatile bool ExtIntTripped = false; // Global for the external interrupt
volatile uint16_t ExtInt_count = 0; // Global for the external interrupt

Margay* Margay::selfPointer;

Margay::Margay(board model_, build specs_)
{
	if(model_ == 2) {
		SD_CS = 4;
		BuiltInLED = 20;
		RedLED = 13;
		GreenLED = 15;
		BlueLED = 14;

		VRef_Pin = 3;
		ThermSense_Pin = 1;
		BatSense_Pin = 2;

		// VSwitch_Pin = 3;
		VSwitch_Pin = 12; //DEBUG!??
		SD_CD = 1;

		Ext3v3Ctrl = 22;
		I2C_SW = 21;
		PG = 18;
		TX = 11;
		RX = 10;
		ExtInt = 11; //Legacy inclusion
		RTCInt = 2;
		LogInt = 28; //ADJUST TO USE PC INT!!!!

		WDHold = 23;
		BatSwitch = 19;

		BatteryDivider = 2.0;

		if(specs_ == BUILD_A) {
			NumADR_OB = 1; //Only check for clock presance
		}

		else if(specs_ == BUILD_B) {
			NumADR_OB = 2; //Tell system to search additional ADRs
			I2C_ADR_OB[1] = 0x69; //Use 0x69 on board ADC (MCP3421A1)
		}

		else if(specs_ == BUILD_C) {
			NumADR_OB = 2; //Tell system to search additional ADRs
			I2C_ADR_OB[1] = 0x6B; //Use 0x6B on board ADC (MCP3421A3)
		}
	}
	else if(model_ == 1) {
		SD_CS = 4;
		BuiltInLED = 20;
		RedLED = 13;
		GreenLED = 15;
		BlueLED = 14;

		VRef_Pin = 2;
		ThermSense_Pin = 1;
		BatSense_Pin = 0;

		// VSwitch_Pin = 3;
		VSwitch_Pin = 12; //DEBUG!??
		SD_CD = 1;

		Ext3v3Ctrl = 19;
		I2C_SW = 21;
		PG = 18;
		TX = 11;
		RX = 10;
		ExtIntPin = 11;
		RTCInt = 10;
		LogInt = 2;

		WDHold = 255; //Null pins
		BatSwitch = 255; //Null pins

		BatteryDivider = 2.0;

		if(specs_ == BUILD_A) {
			NumADR_OB = 1; //Only check for clock presance
		}

		else if(specs_ == BUILD_B) {
			NumADR_OB = 2; //Tell system to search additional ADRs
			I2C_ADR_OB[1] = 0x69; //Use 0x69 on board ADC (MCP3421A1)
		}

		else if(specs_ == BUILD_C) {
			NumADR_OB = 2; //Tell system to search additional ADRs
			I2C_ADR_OB[1] = 0x6B; //Use 0x6B on board ADC (MCP3421A3)
		}


		else if(specs_ == BUILD_D) {
			NumADR_OB = 2; //Tell system to search additional ADRs
			I2C_ADR_OB[1] = 0x6A; //Use 0x6B on board ADC (MCP3421A3)
		}
	}
	else {
		SD_CS = 4;
		BuiltInLED = 19;
		RedLED = 13;
		GreenLED = 15;
		BlueLED = 14;

		VRef_Pin = 2;
		ThermSense_Pin = 1;
		BatSense_Pin = 0;

		VSwitch_Pin = 3;
		SD_CD = 1;

		Ext3v3Ctrl = 12;
		I2C_SW = 255;
		PG = 18;
		ExtIntPin = 11;
		RTCInt = 10;
		LogInt = 2;
		BatteryDivider = 9.0;

		if(specs_ == BUILD_A) {  //Setup sub builds
			NumADR_OB = 1; //Only check for clock presance
		}

		else if(specs_ == BUILD_B) {
			NumADR_OB = 2; //Tell system to search additional ADRs
			I2C_ADR_OB[1] = 0x69; //Use 0x69 on board ADC (MCP3421A1)
		}

		else if(specs_ == BUILD_C) {
			NumADR_OB = 2; //Tell system to search additional ADRs
			I2C_ADR_OB[1] = 0x6B; //Use 0x6B on board ADC (MCP3421A3)
		}
	}

	Model = model_; //Store model info locally
	Specs = specs_; //Store build info locally
}

int Margay::begin(uint8_t *Vals, uint8_t numVals, String header_)
{
	powerOB(ON);  //Turn on on-board power
	powerAux(ON); //Turn on external auxilary power
	pinMode(WDHold, OUTPUT);
	// pinMode(BatSwitch, OUTPUT);
	// digitalWrite(BatSwitch, HIGH);
	// pinMode(Ext3v3Ctrl, OUTPUT);
	// digitalWrite(Ext3v3Ctrl, HIGH); //Make sure external power is on

	pinMode(BuiltInLED, OUTPUT);
	digitalWrite(BuiltInLED, LOW); //Turn built in LED on

	pinMode(VSwitch_Pin, OUTPUT); //Setup switch control as output

	memcpy(I2C_ADR, Vals, sizeof(I2C_ADR)); //Copy array
	// memcpy(I2C_ADR, Vals, numVals); //Copy array  //DEBUG??
	NumADR = numVals; //Copy length of array
	if (ExtIntPin == 255)
	{
		Header = header_; //Copy user defined header
	}
	else
	{
		Header = header_ + ext_int_header_entry;
  }

	// NumADR_OB = 2; //DEBUG!
	RTC.begin(); //Initalize RTC
	RTC.clearAlarm(); //
	adc.Begin(I2C_ADR_OB[1]); //Initalize external ADC
	adc.SetResolution(18);
	EnviroSense.begin(0x77); //Initalize onboard temp/pressure/RH sensor (BME280)


	ADCSRA = 0b10000111; //Confiure on board ADC for low speed, and enable

	Serial.begin(38400); //DEBUG!
	Serial.print("Lib = ");
	Serial.println(LibVersion);
	Serial.print("SN = ");
	int EEPROMLen = EEPROM.length(); //Copy value for faster access
	int val = 0; //Value to read temp EEPROM values into
	int Pos = 0; //used to keep track of position in SN string
	for(int i = EEPROMLen - 8; i < EEPROMLen; i++) {  //Read out Serial Number
		val = EEPROM.read(i);  //Read SN values as individual bytes from EEPROM
		SN[Pos++] = HexMap[(val >> 4)]; //Load upper nibble of hex value, post inc pos
		SN[Pos++] = HexMap[(val % 0x10)]; //Load lower nibble of hex value, post inc pos
		if(i % 2 == 1 && i < EEPROMLen - 1) {
			SN[Pos++] = '-';  //Place - between each SN category, post inc pos
			// Pos += 1;
		}
		SN[19] = NULL; //Null terminate string
	}

	Serial.print(SN); //Print compiled string
	Serial.print("\n\n");
	Serial.println("\nInitializing...\n"); //DEBUG!
	delay(100);
	if(Serial.available()) {  //If time setting info available
		String DateTimeTemp = Serial.readString();
		Serial.println(DateTimeTemp);  //DEBUG!
		int DateTimeVals[6] = {0};
		for(int i = 0; i < 6; i++) {
			DateTimeVals[i] = DateTimeTemp.substring(2*i, 2*(i+1)).toInt();
			Serial.print(i); Serial.print("  "); Serial.println(DateTimeVals[i]);  //DEBUG!
		}
		RTC.setTime(2000 + DateTimeVals[0], DateTimeVals[1], DateTimeVals[2], DateTimeVals[3], DateTimeVals[4], DateTimeVals[5]);
	}

	getTime(); //Get time to pass to computer
	Serial.print("\nTimestamp = ");
	Serial.println(LogTimeDate);

	//Sets up basic initialization required for the system
	selfPointer = this;


	pinMode(RedLED, OUTPUT);
	pinMode(GreenLED, OUTPUT);
	pinMode(BlueLED, OUTPUT);

	LED_Color(OFF);

	// Wire.begin();
	pinMode(SD_CS, OUTPUT);
	// SPI.setDataMode(SPI_MODE0);
	// SPI.setClockDivider(SPI_CLOCK_DIV2); //Sts SPI clock to 4 MHz for an 8 MHz system clock

	SdFile::dateTimeCallback(dateTimeSD); //Setup SD file time setting
	attachInterrupt(digitalPinToInterrupt(RTCInt), Margay::isr1, FALLING); //Attach an interrupt driven by the interrupt from RTC, logs data
	if(Model < 2) attachInterrupt(digitalPinToInterrupt(LogInt), Margay::isr0, FALLING);	//Attach an interrupt driven by the manual log button, sets logging flag and logs data
	// AttachPCI(LogInt, buttonLog, FALLING); //Attach an interrupt driven by the manual log button, sets logging flag and logs data (using pin change interrupts)
	// enableInterrupt(LogInt, buttonLog, FALLING);
	else { //If using Model > v2.2, use PCINT for log button
		*digitalPinToPCMSK(LogInt) |= bit (digitalPinToPCMSKbit(LogInt));  // enable pin
		// PCIFR  |= 0xFE; // clear any outstanding interrupt
		// PCICR  |= 0x01; // enable interrupt for the group
		PCIFR  |= bit (digitalPinToPCICRbit(LogInt)); // clear any outstanding interrupt
	    PCICR  |= bit (digitalPinToPCICRbit(LogInt)); // enable interrupt for the group
	}
	pinMode(RTCInt, INPUT_PULLUP);
	pinMode(LogInt, INPUT);

	I2Ctest();
	clockTest();
	SDtest();
	batTest();
	if(Model >= MODEL_2v0) enviroStats();  //Only print out enviromental variables if BME is on board



  	digitalWrite(BuiltInLED, HIGH);

  	if(OBError) {
  		LED_Color(RED);	//On board failure
  		delay(2000);
  	}
	if(SensorError) {
		LED_Color(ORANGE);  //Sensor failure
		delay(2000);
	}
	if(TimeError) {
		LED_Color(CYAN); //Time set error
		delay(2000);
	}
	if(SDError) {
		LED_Color(PURPLE); //Sd card not inserted
		delay(2000);
	}
	if(BatError) {  //Battery voltage is below level where hardware functionality can be gaurenteed
		for(int i = 0; i < 10; i++) {
			LED_Color(RED);
			delay(100);
			LED_Color(OFF);
			delay(100);
		}
	}

	if(BatWarning && !BatError) {  //Battery charge % is at a concerning level, recomend repacing batteries
		for(int i = 0; i < 10; i++) {
			LED_Color(GOLD); //Sd card not inserted
			delay(100);
			LED_Color(OFF);
			delay(100);
		}
	}
	if(!OBError && !SensorError && !TimeError && !SDError) {  //Include battery error in test??
		LED_Color(GREEN);
		delay(2000);
	}

	Serial.print("\nReady to Log...\n\n");
	NewLog = true; //Set flag to begin new log file

	// delay(2000);

	if (ExtIntPin != 255)
	{
	  pinMode(ExtIntPin, INPUT);
	  digitalWrite(ExtIntPin, HIGH);
    attachInterrupt(digitalPinToInterrupt(ExtIntPin), Margay::isr2, FALLING);
  }

	LED_Color(OFF);
}

ISR (PCINT0_vect) // handle pin change interrupt for D8 to D13 here
{
	// boolean PinVal = (PINA & digitalPinToBitMask(28));
    // if(PinVal == LOW) manualLog = true; //Set flag to manually record an additional data point; //Only fun the function if trigger criteria is true
    // digitalWrite(14, LOW); //DEBUG!
    manualLog = true; //DEBUG!
}

int Margay::begin(String header_)
{
	uint8_t Dummy[1] = {NULL};
	begin(Dummy, 0, header_); //Call generalized begin function
}

void Margay::I2Ctest()
{
	externalI2C(ON);
	int Error = 0;
	bool I2C_Test = true;

	Serial.print("I2C: ");
	for(int i = 0; i < NumADR; i++) {
		Wire.beginTransmission(I2C_ADR[i]);
    	Error = Wire.endTransmission();
    	if(Error != 0) {
    		if(I2C_Test) Serial.println(" Fail");
    		Serial.print("   Fail At: ");
    		Serial.println(I2C_ADR[i], HEX);
    		I2C_Test = false;
    		SensorError = true;
		}
	}

	//Switch to connect to onboard I2C!
	externalI2C(OFF);

	for(int i = 0; i < NumADR_OB; i++) {
		Wire.beginTransmission(I2C_ADR_OB[i]);
    	Error = Wire.endTransmission();
    	if(Error != 0) {
    		if(I2C_Test) Serial.println(" Fail");
    		Serial.print("   Fail At: ");
    		Serial.println(I2C_ADR_OB[i], HEX);
    		I2C_Test = false;
    		OBError = true;
		}
	}

	if(I2C_Test) Serial.println("PASS");
}


void Margay::SDtest()
{
	bool SDErrorTemp = false;
	// bool SD_Test = true;

  // SD_CD is pulled up: HIGH=1 if not present
  // SD card being inserted closes a switch to pull it LOW
	pinMode(SD_CD, INPUT);
	bool CardNotPresent = digitalRead(SD_CD);

	Serial.print("SD: ");
	delay(5); //DEBUG!
	if(CardNotPresent) {
		Serial.println(F(" NO CARD"));
  	SDErrorTemp = true;
  	SDError = true; //Card not inserted
	}
	else if (!SD.begin(SD_CS)) {
  	OBError = true;
  	SDErrorTemp = true;
	}

  // If card is present, do the following:
	if(!CardNotPresent) {
		SD.mkdir("NW");  //Create NW folder (if not already present)
		SD.chdir("/NW"); //Move file pointer into NW folder (at root level)
		SD.mkdir(SN); //Make directory with serial number as name
		SD.chdir(SN); //Move into this directory
		//Change directory to SN# named dir
		SD.mkdir("Logs"); //Use???
	  String FileNameTest = "HWTest";
	  (FileNameTest + ".txt").toCharArray(FileNameTestC, 11);
	  SD.remove(FileNameTestC); //Remove any previous files

	  randomSeed(analogRead(A7)); //Seed with a random process to try to endsure randomness
	  int RandVal = random(30557); //Generate a random number between 0 and 30557 (the number of words in Hamlet)
	  char RandDigits[6] = {0};
	  sprintf(RandDigits, "%d", RandVal); //Convert RandVal into a series of digits
	  int RandLength = (int)((ceil(log10(RandVal))+1)*sizeof(char)); //Find the length of the values in the array
	  File DataWrite = SD.open(FileNameTestC, FILE_WRITE);
	  if(DataWrite) {
		  DataWrite.println(RandVal);
		  DataWrite.println("\nHe was a man. Take him for all in all.");
		  DataWrite.println("I shall not look upon his like again.");
		  DataWrite.println("-Hamlet, Act 1, Scene 2");
	  }
	  DataWrite.close();
	  char TestDigits[6] = {0};
	  File DataRead = SD.open(FileNameTestC, FILE_READ);
	  if(DataRead) {
    	DataRead.read(TestDigits, RandLength);
		  for(int i = 0; i < RandLength - 1; i++){ //Test random value string
		    if(TestDigits[i] != RandDigits[i]) {
		      SDErrorTemp = true;
		      OBError = true;
		    }
		  }
	  }
	  DataRead.close();

	  keep_SPCR=SPCR;
  }

	if(SDError && !CardNotPresent) Serial.println("FAIL");  //If card is inserted and still does not connect propperly, throw error
	else if(!SDError && !CardNotPresent) Serial.println("PASS");  //If card is inserted AND connectects propely return success
}

void Margay::clockTest()
{
	int Error = 1;
	uint8_t TestSeconds = 0;
	bool OscStop = false;

	Serial.print("Clock: ");
	Wire.beginTransmission(I2C_ADR_OB[0]);
  	Wire.write(0xFF);
	Error = Wire.endTransmission();

	if(Error == 0) {
		getTime(); //FIX!
		TestSeconds = RTC.getValue(5);
	  	delay(1100);
	  	if(RTC.getValue(5) == TestSeconds) {
	  		OBError = true; //If clock is not incrementing
	  		OscStop = true; //Oscilator not running
	  	}
	}

	unsigned int YearNow = RTC.getValue(0);

	if(YearNow == 00) {  //If value is 2000, work around Y2K bug by setting time to Jan 1st, midnight, 2049
		// if(YearNow <= 00) RTC.setTime(2018, 01, 01, 00, 00, 00);  //Only reset if Y2K
		// getTime(); //Update local time
		TimeError = true;
		Serial.println(" PASS, BAD TIME");
	}

	if(Error != 0) {
		Serial.println(" FAIL");
		OBError = true;
	}

	else if(Error == 0 && OscStop == false && TimeError == false) {
		Serial.println(" PASS");
	}
}

void Margay::batTest()
{
	if(getBatVoltage() < BatVoltageError) BatError = true; //Set error flag if below min voltage
	if(getBatPer() < BatPercentageWarning) BatWarning = true; //Set warning flag is below set percentage
	Serial.print("Bat = ");
	Serial.print(getBatVoltage());
	Serial.print("V\t");
	Serial.print(getBatPer());
	Serial.println("%");
}
void Margay::powerTest()
{
	int Error = 0;

	digitalWrite(Ext3v3Ctrl, HIGH); //Turn off power to outputs

	Serial.print("Power: ");
	Wire.beginTransmission(I2C_ADR[1]);
	Error = Wire.endTransmission();
	if(Error == 0) Serial.println(" FAIL");

	if(Error != 0) Serial.println(" PASS");

	digitalWrite(Ext3v3Ctrl, LOW); //Turn power back on
}

void Margay::enviroStats()
{
	Serial.print("Temp = ");
	Serial.print(EnviroSense.GetTemperature());
	Serial.println("C");
	Serial.print("Pressure = ");
	Serial.print(EnviroSense.GetPressure());
	Serial.println(" mBar");
	Serial.print("RH = ");
	Serial.print(EnviroSense.GetHumidity());
	Serial.println("%");
}

void Margay::initLogFile()
{
	// SD.chdir("/"); //Return to root to define starting state
	SD.chdir("/NW");  //Move into northern widget folder from root
	SD.chdir(SN);  //Move into specific numbered sub folder
	SD.chdir("Logs"); //Move into the logs sub-folder
	//Perform same search, but do so inside of "SD:NW/sn/Logs"
    char NumCharArray[6];
    String FileName = "Log";
    int FileNum = 1;
    sprintf(NumCharArray, "%05d", FileNum);
    //Serial.print("NumString: ");
    //Serial.println(ns);
    (FileName + String(NumCharArray) + ".txt").toCharArray(FileNameC, 13);
    while(SD.exists(FileNameC)) {
      FileNum += 1;
      sprintf(NumCharArray, "%05d", FileNum);
      (FileName + String(NumCharArray) + ".txt").toCharArray(FileNameC, 13);
    }
    Serial.print("FileNameC: ");
    Serial.println(FileNameC);
  	String InitData = "Lib = " + String(LibVersion) + " SN = " + String(SN);  //Make string of onboard characteristics
  	logStr(InitData); //Log as first line of data
  	// logStr("Drink. Drink. Drink. Drink. Don't Think. Drive. Kill. Get drunk a lot. And work 40 hours a week. Drink. Drink. Drink. Drink. Don't Think. Drive. Kill. Get drunk a lot. And work 40 hours a week. "); //DEBUG!
    // logStr(Header); //DEBUG!
    if(Model < MODEL_2v0) logStr("Time [UTC], Temp OB [C], Temp RTC [C], Bat [V], " + Header); //Log concatonated header (for old loggers)
    else logStr("Time [UTC], PresOB [mBar], RH_OB [%], TempOB [C], Temp RTC [C], Bat [V], " + Header); //Log concatonated header (for new loggers)
    // logStr("Time [UTC], PresOB [mBar], RH_OB [%], TempOB [C], Temp RTC [C], Bat [V], " + Header); //Log concatonated header (for new loggers)
}

int Margay::logStr(String val)
{
	Serial.println(val); //Echo to serial monitor
	// SD.begin(SD_CS); //DEBUG!
	// SD.chdir("/"); //Return to root to define starting state
	SD.chdir("/NW");  //Move into northern widget folder from root
	SD.chdir(SN);  //Move into specific numbered sub folder
	SD.chdir("Logs"); //Move into the logs sub-folder
	File DataFile = SD.open(FileNameC, FILE_WRITE);

	// if the file is available, write to it:
	if (DataFile) {
		DataFile.println(val);
	   // return 0;
	}
	// if the file isn't open, pop up an error:
	else {
	   // return -1;
	}

	DataFile.close();
}

void Margay::LED_Color(unsigned long val) //Set color of onboard led
{
	int Red = 0; //Red led color
	int Green = 0;  //Green led color
	int Blue = 0;  //Blue led color
	int Lum = 0;  //Luminosity

	//Parse all values from single val
	Blue = val & 0xFF;
	Green = (val >> 8) & 0xFF;
	Red = (val >> 16) & 0xFF;
	Lum = (val >> 24) & 0xFF;
	//  Lum = 255 - Lum; //Invert since LEDs are open drain

	analogWrite(RedLED, 255 - (Red * Lum)/0xFF);
	analogWrite(GreenLED, 255 - (Green * Lum)/0xFF);
	analogWrite(BlueLED, 255 - (Blue * Lum)/0xFF);
}

void Margay::getTime()
{
	//Update global time string
	// DateTime TimeStamp = RTC.now();
	// LogTimeDate = String(TimeStamp.year()) + "/" + String(TimeStamp.month()) + "/" + String(TimeStamp.day()) + " " + String(TimeStamp.hour()) + ":" + String(TimeStamp.minute()) + ":" + String(TimeStamp.second());
	LogTimeDate = RTC.getTime(0);
}

float Margay::getTemp(temp_val val)
{
	float Vcc = 3.3;
	if(val == therm_val) {
		float val = float(analogRead(ThermSense_Pin))*(Vcc/1024.0);
		float TempData = tempConvert(val, Vcc, 10000.0, A, B, C, D, 10000.0);
		TempData = TempData - 273.15; //Get temp from on board thermistor
		return TempData;
	}
	if(val == RTC_val) {
		float RTCTemp = RTC.getTemp();  //Get Temp from RTC
		return RTCTemp;
	}
}

float Margay::getBatVoltage()
{
	ADMUX = 0b00000000; //Setup voltage ref
	delay(10); //Alow for >1 clock cycle to set values
	ADCSRA = 0b10000111; //Enable ADC, set clock divider to max to deal with high impedance input
	delay(10); //Alow for >1 clock cycle to set values
	float Vcc = 3.3;
	float BatVoltage = analogRead(BatSense_Pin); //Get (divided) battery voltage
	float Comp = (1.8/3.3)*1024.0/analogRead(VRef_Pin);  //Find compensation value with VRef due to Vcc error
	if(Model == 0) Comp = 1.0; //Overide comp calculation since many v0.0 models do not have ref equiped
	BatVoltage = BatVoltage*BatteryDivider*Comp*(Vcc/1024.0); //Compensate for voltage divider and ref voltage error
	return BatVoltage;
}

float Margay::getBatPer()
{
	//NOTE: Fit developed for Duracell AA, should work well for most alkalines, but no gaurentee given on accuracy
	//From 305 to 100% capacity, should be accurate to within 1% (for data taken at 25C)
	float A = -1.9809;
	float B = 6.2931;
	float C = -4.0063;
	float val = getBatVoltage()/3.0; //Divide to get cell voltage
	float Per = ((A*pow(val, 2) + B*val + C)*2 - 1)*100.0; //Return percentage of remaining battery energy
	if(Per < 0) return 0;  //Do not allow return of non-sensical values
	if(Per > 100) return 100;  //Is this appropriate? Float voltage could be higher than specified and still be correct
	return Per;
}

String Margay::getOnBoardVals()
{
	//Get onboard temp, RTC temp, and battery voltage, referance voltage
	// float VRef = analogRead(VRef_Pin);
	float Vcc = 3.3; //(1.8/VRef)*3.3; //Compensate for Vcc using VRef
	// Serial.println(Vcc); //DEBUG!
	float TempData = 0; //FIX!!! Dumb!

	if(Model < MODEL_2v0) {  //For older thermistor models
		float val = float(analogRead(ThermSense_Pin));
		float Comp = (1.8/3.3)*1024.0/analogRead(VRef_Pin);  //Find compensation value with VRef due to Vcc error
		if(Model == 0) Comp = 1.0; //Overide comp calculation since many v0.0 models do not have ref equiped
		val = val*Comp*(Vcc/1024.0); //Compensate for ref voltage error
		//  float Vout = Vcc - val;
		//  Serial.println(val); //DEBUG!
		//  Serial.println(Vout);  //DEBUG!
		TempData = tempConvert(val, Vcc*Comp, 10000.0, A, B, C, D, 10000.0);
		TempData = TempData - 273.15; //Get temp from on board thermistor
	}

	// delay(10);
	float BatVoltage = getBatVoltage(); //Get battery voltage, Include voltage divider in math

	// Temp[3] = Clock.getTemperature(); //Get tempreture from RTC //FIX!
	float RTCTemp = RTC.getTemp();  //Get Temp from RTC
	getTime(); //FIX!
	if(Model< MODEL_2v0) return LogTimeDate + "," + String(TempData) + "," + String(RTCTemp) + "," + String(BatVoltage) + ",";
	else return LogTimeDate + "," + String(EnviroSense.GetString()) + String(RTCTemp) + "," + String(BatVoltage) + ",";
}

float Margay::tempConvert(float V, float Vcc, float R, float A, float B, float C, float D, float R25)
{
	//  Serial.print("R = "); //DEBUG!
	//  Serial.println(R); //DEBUG!
	float Rt = ((Vcc/V)*R) - R;
	//  Serial.print("Rt = "); //DEBUG!
	//  Serial.println(Rt); //DEBUG!
	float LogRt = log(Rt/R25);
	//  Serial.print("LogRt = "); //DEBUG!
	//  Serial.println(LogRt); //DEBUG!
	float T = 1.0/(A + B*LogRt + C*pow(LogRt, 2.0) + D*pow(LogRt, 3.0));
	return T;
}

void Margay::blinkGood()
{
  // Peppy blinky pattern to show that the logger has successfully initialized
  digitalWrite(BlueLED,LOW);
  delay(651);
  digitalWrite(BlueLED,HIGH);
  delay(300);
  digitalWrite(BlueLED,LOW);
  delay(100);
  digitalWrite(BlueLED,HIGH);
  delay(200);
  digitalWrite(BlueLED,LOW);
  delay(100);
  digitalWrite(BlueLED,HIGH);
}

float Margay::getVoltage()  //Get voltage from Ax pin
{
	float val = adc.GetVoltage();
	return val;
}

void Margay::run(String (*update)(void), unsigned long logInterval) //Pass in function which returns string of data
{
	// Serial.println("BANG!"); //DEBUG!
	// Serial.println(millis()); //DEBUG!
	if(NewLog) {
		// Serial.println("Log Started!"); //DEBUG
		// LogEvent = true;
		// unsigned long TempLogInterval = logInterval; //ANDY, Fix with addition of function??
		RTC.setAlarm(logInterval); //DEBUG!
		initLogFile(); //Start a new file each time log button is pressed

		//Add inital data point
		addDataPoint(update);
		NewLog = false;  //Clear flag once log is started
    	blinkGood();  //Alert user to start of log
    	resetWDT(); //Clear alarm
	}

	if(LogEvent) {
		// Serial.println("Log!"); //DEBUG!
		// RTC.setAlarm(logInterval);  //Set/reset alarm //DEBUG!
		addDataPoint(update); //Write values to SD
		LogEvent = false; //Clear log flag
		// Serial.println("BANG!"); //DEBUG!
		RTC.setAlarm(logInterval);  //Set/reset alarm
		// Serial.println("ResetTimer"); //DEBUG!
		resetWDT(); //Clear alarm
	}

	if(manualLog) {  //Write data to SD card without interrupting existing timing cycle
		// Serial.println("Click!"); //DEBUG!
		addDataPoint(update); //write values to SD
		manualLog = false; //Clear log flag
		resetWDT(); //Clear alarm
	}

	if(ExtIntTripped) {  // Defaults to just counter for now
		// Serial.println("TIP!"); //DEBUG!
    ExtInt_count ++;
		ExtIntTripped = false; // Clear interrupt flag flag
		resetWDT(); //Clear alarm
		delay(150); //Hard-code for now; tipping bucket "debounce"
    attachInterrupt(digitalPinToInterrupt(ExtIntPin), Margay::isr2, FALLING);
	}

	if(!digitalRead(RTCInt)) {  //Catch alarm if not reset properly
   		Serial.println("Reset Alarm"); //DEBUG!
		RTC.setAlarm(logInterval); //Turn alarm back on
	}

	AwakeCount++;

	if(AwakeCount > 5) {
	//    AwakeCount = 0;
		// Serial.println(millis()); //DEBUG!
		sleepNow();
	}
	delay(1);
}

void Margay::resetWDT()  //Send a pulse to "feed" the watchdog timer
{
	digitalWrite(WDHold, HIGH); //Set DONE pin high
	delayMicroseconds(5); //Wait a short pulse
	digitalWrite(WDHold, LOW);
}

void Margay::externalI2C(bool state)
{
	/*
	Must be ON to read off-board sensors, OFF to read on-board sensors and RTC
	Serves to isolate these s.t. I2C addresses may not clash.
	*/
	pinMode(I2C_SW, OUTPUT);
	if ( state == ON ){
		digitalWrite(I2C_SW, HIGH);
	}
	else{
		digitalWrite(I2C_SW, LOW);
	}
	delay(1); // Any time needed to switch states; may not be necessary
}

void Margay::addDataPoint(String (*update)(void)) //Reads new data and writes data to SD
{
	String data = "";
	if(Model >= MODEL_2v0) EnviroSense.begin(0x77); //Re-initialize BME280  //FIX??
	// Serial.println("Called Update"); //DEBUG!
	externalI2C(ON);
	data = (*update)(); //Run external update function
	externalI2C(OFF);
	// Serial.println("Request OB Vals"); //DEBUG!
	// Briefly flash an LED to show that data are being logged
	// without needing to waste extra time/power with a delay.
	// This step should always take the same amount of time
	// unless there is a significant library or xtal change
	pinMode(BlueLED, OUTPUT);
	digitalWrite(BlueLED, LOW); //ON
	data = getOnBoardVals() + data; //Prepend on board readings
	digitalWrite(BlueLED, HIGH); //OFF
	// Serial.println("Got OB Vals");  //DEBUG!
	logStr(data);
	// Serial.println("Loged Data"); //DEBUG!
}

//ISRs

void Margay::buttonLog()
{
	//ISR to respond to pressing log button and waking device from sleep and starting log
	manualLog = true; //Set flag to manually record an additional data point
}

void Margay::extIntCounter()
{
  // ISR for an external event waking the logger
  detachInterrupt(digitalPinToInterrupt(ExtIntPin));
  ExtIntTripped = true; // Set flag to just increment the counter and return to sleep
}

void Margay::writeDataToSD()
{
	//Write global data to SD
	LogEvent = true; //Set flag for a log event
	AwakeCount = 0;
}

// ExtInt functions
void Margay::setExtInt(uint8_t n, String header_entry)
{
  ExtIntPin = n;
  ext_int_header_entry = header_entry;
}

uint16_t Margay::getExtIntCount(bool reset0)
{
  uint16_t out = ExtInt_count;
  if (reset0)
  {
    resetExtIntCount(0);
  }
  return out;
}

void Margay::resetExtIntCount(uint16_t start)
{
    ExtInt_count = start;
}

void Margay::powerAux(bool State)
{
	pinMode(Ext3v3Ctrl, OUTPUT); //Setup outputs for robustness
	if(State) powerOB(ON); //Turn on on-board power if required
	if(Model >= MODEL_2v0) {  //use positive logic for Model v2.0 and newer
		digitalWrite(Ext3v3Ctrl, State); //Switch 3v3 Aux power
	}
	else digitalWrite(Ext3v3Ctrl, !State); //Switch 3v3 Aux power
}

void Margay::powerOB(bool State)
{
	pinMode(BatSwitch, OUTPUT);
	digitalWrite(BatSwitch, State); //Set bat switch for onboard 3v3/main power
}

void Margay::dateTimeSD(uint16_t* date, uint16_t* time)
{
	// DateTime now = RTC.now();
	// sprintf(timestamp, "%02d:%02d:%02d %2d/%2d/%2d \n", now.hour(),now.minute(),now.second(),now.month(),now.day(),now.year()-2000);
	// Serial.println("yy");
	// Serial.println(timestamp);
	// return date using FAT_DATE macro to format fields
	// Serial.println(selfPointer->RTC.getValue(0)); //DEBUG!
	*date = FAT_DATE(selfPointer->RTC.getValue(0) + 2000, selfPointer->RTC.getValue(1), selfPointer->RTC.getValue(2));

	// return time using FAT_TIME macro to format fields
	*time = FAT_TIME(selfPointer->RTC.getValue(3), selfPointer->RTC.getValue(4), selfPointer->RTC.getValue(5));
}

void Margay::dateTimeSD_Glob(uint16_t* date, uint16_t* time) {selfPointer->dateTimeSD(date, time);}  //Fix dumb name!

void Margay::isr0() { selfPointer->buttonLog(); }

// ISR(PCINT0_vect)
// {
// 	manualLog = true;
// }

// ISR(PCINT0_vect) {
void Margay::isr1() { selfPointer->writeDataToSD(); }
void Margay::isr2() { selfPointer->extIntCounter(); }

//Low Power functions
void Margay::sleepNow()         // here we put the arduino to sleep
{
    /* Now is the time to set the sleep mode. In the Atmega8 datasheet
     * http://www.atmel.com/dyn/resources/prod_documents/doc2486.pdf on page 35
     * there is a list of sleep modes which explains which clocks and
     * wake up sources are available in which sleep mode.
     *
     * In the avr/sleep.h file, the call names of these sleep modes are to be found:
     *
     * The 5 different modes are:
     *     SLEEP_MODE_IDLE         -the least power savings
     *     SLEEP_MODE_ADC
     *     SLEEP_MODE_PWR_SAVE
     *     SLEEP_MODE_STANDBY
     *     SLEEP_MODE_PWR_DOWN     -the most power savings
     *
     * For now, we want as much power savings as possible, so we
     * choose the according
     * sleep mode: SLEEP_MODE_PWR_DOWN
     *
     */
    // MCUCR = bit (BODS) | bit (BODSE);
    // MCUCR = bit (BODS);
//  wdt_disable();  //DEBUG!??
  // power_adc_disable(); // ADC converter
  // // power_spi_disable(); // SPI
  // power_usart0_disable();// Serial (USART)
  // power_timer1_disable();// Timer 1
  // power_timer2_disable();// Timer 2
  // ADCSRA = 0;
  turnOffSDcard();
  // digitalWrite(Ext3v3Ctrl, HIGH); //Turn off extenral rail
  // SPI.end(); //Turn off SPI
  // digitalWrite(SD_CS, LOW);
  // pinMode(SD_CS, INPUT); //Disconnect SD chip slect pin
  // pinMode(5, INPUT); //Set all SPI pins as inputs, will be reversed be beginning SPI again
  // pinMode(6, INPUT);
  // pinMode(7, INPUT);
  	digitalWrite(VSwitch_Pin, LOW); //DEBUG!
  	keep_ADCSRA = ADCSRA;
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);   // sleep mode is set here
    cbi(ADCSRA,ADEN);
    sleep_enable();
    sleep_bod_disable();
    sei();

    sleep_cpu();
    sleep_disable();
	// pinMode(3, OUTPUT); //DEBUG!
    // detachInterrupt(0);      // disables interrupt 0 on pin 2 so the
//    ADCSRA = 1; //Turn ADC back on
    // digitalWrite(Ext3v3Ctrl, LOW); //turn external rail back on
    // digitalWrite(SD_CS, HIGH);
//     SPI.begin();
    turnOnSDcard();
	ADCSRA = 135; //DEBUG!
	// digitalWrite(VSwitch_Pin, HIGH);  //DEBUG!
    // pinMode(SD_CS, OUTPUT); //Disconnect SD chip slect pin

}

void Margay::turnOffSDcard()
{
delay(6);
                                       // disable SPI
// power_spi_disable();                     // disable SPI clock
// DDRB &= ~((1<<DDB5) | (1<<DDB7) | (1<<DDB6) | (1<<DDB4));   // set All SPI pins to INPUT
// pinMode(SD_CD, INPUT);
// DDRC &= ~((1<<DDC0) | (1<<DDC1));
pinMode(16, INPUT);
pinMode(17, INPUT);
// digitalWrite(8, LOW);
// digitalWrite(9, LOW);
pinMode(8, INPUT);
pinMode(9, INPUT);
// digitalWrite(16, HIGH);
// digitalWrite(17, HIGH);
//digitalWrite(SD_CS, HIGH);
// digitalWrite(5, LOW);
// // Note: you must disconnect the LED on pin 13 or you’ll bleed current through the limit resistor
// // LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF); // wait 1 second before pulling the plug!
delay(6);
// digitalWrite(Ext3v3Ctrl, HIGH); //MODEL <= v1
// digitalWrite(Ext3v3Ctrl, LOW);  //turn off external 3v3 rail
// digitalWrite(BatSwitch, LOW); //Turn off battery connection to sense divider
powerAux(OFF); //turn off external 3v3 rail
powerOB(OFF); //Turn off battery connection to sense divider
// digitalWrite(BatRailCtrl, HIGH);
delay(1);
digitalWrite(SD_CS, LOW);
delay(20);
// SPCR = SPCR & 0b11101111;
SPCR = 0;
power_spi_disable();
// SPI.end();
delay(10);
// pinMode(5, OUTPUT);d
// digitalWrite(5, LOW);
// DDRB &= ~((1<<DDB5));
// PORTB &= ~(1<<PORTB5); //Set port B5 (MOSI) LOW
// DDRB &= ~((1<<DDB5) | (1<<DDB7) | (1<<DDB6) | (1<<DDB4));
// PORTB |= ((1<<DDB5) | (1<<DDB7) | (1<<DDB6) | (1<<DDB4));     // set ALL SPI pins HIGH (~30k pullup)
// digitalWrite(SD_CS, LOW);
// pinMode(SD_CS, INPUT);
delay(6);
}

void Margay::turnOnSDcard()
{
// pinMode(SD_CS, OUTPUT);
// SPI.begin();
// sd.begin(SD_CS);
// DDRB |= ((1<<DDB5));
// digitalWrite(SD_CS, HIGH);
// digitalWrite(Ext3v3Ctrl, HIGH);  //turn off external 3v3 rail
// digitalWrite(BatSwitch, HIGH); //Turn off battery connection to sense divider
powerOB(ON); //Turn on battery connection to sense divider
powerAux(ON); //turn on external 3v3 rail
delay(6);                                            // let the card settle
// some cards will fail on power-up unless SS is pulled up  ( &  D0/MISO as well? )
// DDRC = DDRC | ((1<<DDC0) | (1<<DDC1));
// DDRB = DDRB | (1<<DDB7) | (1<<DDB5) | (1<<DDB4); // set SCLK(D13), MOSI(D11) & SS(D10) as OUTPUT
// Note: | is an OR operation so  the other pins stay as they were.                (MISO stays as INPUT)
// PORTB = PORTB & ~(1<<DDB7);  // disable pin 13 SCLK pull-up – leave pull-up in place on the other 3 lines
power_spi_enable();                      // enable the SPI clock
SPCR=keep_SPCR;                          // enable SPI peripheral
// delay(20);
// digitalWrite(BatRailCtrl, LOW);
// digitalWrite(Ext3v3Ctrl, LOW); //MODEL <= v1
delay(10);
// digitalWrite(3, HIGH); //DEBUG!
SD.begin(SD_CS, SD_SCK_MHZ(8));
// digitalWrite(3, LOW); //DEBUG!
}
