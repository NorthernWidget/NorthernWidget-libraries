//Margay library

#include <Margay.h>
#include <Arduino.h>

// #include <PCINT.h>



// extern void AttachPCI(byte Pin, void (*UserFunc)(void), int Mode);
// extern void enableInterrupt(uint8_t interruptDesignator, void (*userFunction)(void), uint8_t mode);
// uint8_t LogInt = 28; //DEBUG!

// *digitalPinToPCMSK(LogInt) |= bit (digitalPinToPCMSKbit(LogInt));  // enable pin



volatile bool ManualLog = false; // Global for interrupt access

volatile uint8_t ExtIntPin = 255; // Sets external interrupt number; 255 for none
String ext_int_header_entry;
volatile bool ExtIntTripped = false; // Global for the external interrupt
volatile uint16_t ExtInt_count = 0; // Global for the external interrupt

Margay* Margay::selfPointer;

Margay::Margay(board Model_, build Specs_)
{	
	if(Model_ == 2) {
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
		I2C_SW = 12;
		PG = 18;
		TX = 11;
		RX = 10; 
		ExtInt = 11; //Legacy inclusion 
		RTCInt = 2;
		LogInt = 28; //ADJUST TO USE PC INT!!!! 

		WDHold = 23; 
		BatSwitch = 19; 

		BatteryDivider = 2.0;

		if(Specs_ == Build_A) {
			NumADR_OB = 1; //Only check for clock presance 
		}

		else if(Specs_ == Build_B) {
			NumADR_OB = 2; //Tell system to search additional ADRs
			I2C_ADR_OB[1] = 0x69; //Use 0x69 on board ADC (MCP3421A1)
		}

		else if(Specs_ == Build_C) {
			NumADR_OB = 2; //Tell system to search additional ADRs
			I2C_ADR_OB[1] = 0x6B; //Use 0x6B on board ADC (MCP3421A3)
		}
	}
	else if(Model_ == 1) {
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
		I2C_SW = 12;
		PG = 18;
		TX = 11;
		RX = 10; 
		ExtIntPin = 11;
		RTCInt = 10;
		LogInt = 2; 

		WDHold = 255; //Null pins
		BatSwitch = 255; //Null pins

		BatteryDivider = 2.0;

		if(Specs_ == Build_A) {
			NumADR_OB = 1; //Only check for clock presance 
		}

		else if(Specs_ == Build_B) {
			NumADR_OB = 2; //Tell system to search additional ADRs
			I2C_ADR_OB[1] = 0x69; //Use 0x69 on board ADC (MCP3421A1)
		}

		else if(Specs_ == Build_C) {
			NumADR_OB = 2; //Tell system to search additional ADRs
			I2C_ADR_OB[1] = 0x6B; //Use 0x6B on board ADC (MCP3421A3)
		}


		else if(Specs_ == Build_D) {
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

		if(Specs_ == Build_A) {  //Setup sub builds 
			NumADR_OB = 1; //Only check for clock presance 
		}

		else if(Specs_ == Build_B) {
			NumADR_OB = 2; //Tell system to search additional ADRs
			I2C_ADR_OB[1] = 0x69; //Use 0x69 on board ADC (MCP3421A1)
		}

		else if(Specs_ == Build_C) {
			NumADR_OB = 2; //Tell system to search additional ADRs
			I2C_ADR_OB[1] = 0x6B; //Use 0x6B on board ADC (MCP3421A3)
		}
	}

	Model = Model_; //Store model info locally
	Specs = Specs_; //Store build info locally 
}

int Margay::begin(uint8_t *Vals, uint8_t NumVals, String Header_)
{
	PowerOB(ON);  //Turn on on-board power
	PowerAux(ON); //Turn on external auxilary power 
	pinMode(WDHold, OUTPUT);
	// pinMode(BatSwitch, OUTPUT);
	// digitalWrite(BatSwitch, HIGH);
	// pinMode(Ext3v3Ctrl, OUTPUT);
	// digitalWrite(Ext3v3Ctrl, HIGH); //Make sure external power is on

	pinMode(BuiltInLED, OUTPUT);
	digitalWrite(BuiltInLED, LOW); //Turn built in LED on

	pinMode(VSwitch_Pin, OUTPUT); //Setup switch control as output

	memcpy(I2C_ADR, Vals, sizeof(I2C_ADR)); //Copy array
	// memcpy(I2C_ADR, Vals, NumVals); //Copy array  //DEBUG??
	NumADR = NumVals; //Copy length of array
	if (ExtIntPin == 255)
	{
		Header = Header_; //Copy user defined header
	}
	else
	{
		Header = Header_ + ext_int_header_entry;
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
	int Val = 0; //Value to read temp EEPROM values into
	int Pos = 0; //used to keep track of position in SN string
	for(int i = EEPROMLen - 8; i < EEPROMLen; i++) {  //Read out Serial Number 
		Val = EEPROM.read(i);  //Read SN values as individual bytes from EEPROM
		SN[Pos++] = HexMap[(Val >> 4)]; //Load upper nibble of hex value, post inc pos
		SN[Pos++] = HexMap[(Val % 0x10)]; //Load lower nibble of hex value, post inc pos
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

	GetTime(); //Get time to pass to computer 
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

	SdFile::dateTimeCallback(DateTimeSD); //Setup SD file time setting
	attachInterrupt(digitalPinToInterrupt(RTCInt), Margay::isr1, FALLING); //Attach an interrupt driven by the interrupt from RTC, logs data
	if(Model < 2) attachInterrupt(digitalPinToInterrupt(LogInt), Margay::isr0, FALLING);	//Attach an interrupt driven by the manual log button, sets logging flag and logs data
	// AttachPCI(LogInt, ButtonLog, FALLING); //Attach an interrupt driven by the manual log button, sets logging flag and logs data (using pin change interrupts)
	// enableInterrupt(LogInt, ButtonLog, FALLING);
	else { //If using Model > v2.2, use PCINT for log button
		*digitalPinToPCMSK(LogInt) |= bit (digitalPinToPCMSKbit(LogInt));  // enable pin
		// PCIFR  |= 0xFE; // clear any outstanding interrupt
		// PCICR  |= 0x01; // enable interrupt for the group
		PCIFR  |= bit (digitalPinToPCICRbit(LogInt)); // clear any outstanding interrupt
	    PCICR  |= bit (digitalPinToPCICRbit(LogInt)); // enable interrupt for the group
	}
	pinMode(RTCInt, INPUT_PULLUP);
	pinMode(LogInt, INPUT);

	I2CTest();
	ClockTest();
	SDTest();
	BatTest();
	if(Model >= Model_2v0) EnviroStats();  //Only print out enviromental variables if BME is on board 
	

  	
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
    // if(PinVal == LOW) ManualLog = true; //Set flag to manually record an additional data point; //Only fun the function if trigger criteria is true 
    // digitalWrite(14, LOW); //DEBUG!
    ManualLog = true; //DEBUG!
}

int Margay::begin(String Header_)
{
	uint8_t Dummy[1] = {NULL};
	begin(Dummy, 0, Header_); //Call generalized begin function
}

void Margay::I2CTest() 
{
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


void Margay::SDTest() 
{
	bool SDErrorTemp = false;
	// bool SD_Test = true;

	pinMode(SD_CD, INPUT);
	bool CardPressent = digitalRead(SD_CD);

	Serial.print("SD: ");
	delay(5); //DEBUG!
	if(CardPressent) {
		Serial.println(F(" NO CARD"));
    	SDErrorTemp = true;
    	SDError = true; //Card not inserted
	}
	else if (!SD.begin(SD_CS)) {
    	OBError = true;
    	SDErrorTemp = true;
  	}

  	if(!CardPressent) {
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
  
	if(SDError && !CardPressent) Serial.println("FAIL");  //If card is inserted and still does not connect propperly, throw error
  	else if(!SDError && !CardPressent) Serial.println("PASS");  //If card is inserted AND connectects propely return success 
}

void Margay::ClockTest() 
{ 
	int Error = 1;
	uint8_t TestSeconds = 0;
	bool OscStop = false;

	Serial.print("Clock: ");
	Wire.beginTransmission(I2C_ADR_OB[0]);
  	Wire.write(0xFF);
	Error = Wire.endTransmission();

	if(Error == 0) {
		GetTime(); //FIX!
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
		// GetTime(); //Update local time
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

void Margay::BatTest()
{
	if(GetBatVoltage() < BatVoltageError) BatError = true; //Set error flag if below min voltage
	if(GetBatPer() < BatPercentageWarning) BatWarning = true; //Set warning flag is below set percentage
	Serial.print("Bat = ");
	Serial.print(GetBatVoltage());
	Serial.print("V\t");
	Serial.print(GetBatPer());
	Serial.println("%");
}
void Margay::PowerTest() 
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

void Margay::EnviroStats() 
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

void Margay::InitLogFile()
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
  	LogStr(InitData); //Log as first line of data
  	// LogStr("Drink. Drink. Drink. Drink. Don't Think. Drive. Kill. Get drunk a lot. And work 40 hours a week. Drink. Drink. Drink. Drink. Don't Think. Drive. Kill. Get drunk a lot. And work 40 hours a week. "); //DEBUG!
    // LogStr(Header); //DEBUG!
    if(Model < Model_2v0) LogStr("Time [UTC], Temp OB [C], Temp RTC [C], Bat [V], " + Header); //Log concatonated header (for old loggers)
    else LogStr("Time [UTC], PresOB [mBar], RH_OB [%], TempOB [C], Temp RTC [C], Bat [V], " + Header); //Log concatonated header (for new loggers)
    // LogStr("Time [UTC], PresOB [mBar], RH_OB [%], TempOB [C], Temp RTC [C], Bat [V], " + Header); //Log concatonated header (for new loggers)
}

int Margay::LogStr(String Val) 
{
	Serial.println(Val); //Echo to serial monitor 
	// SD.begin(SD_CS); //DEBUG!
	// SD.chdir("/"); //Return to root to define starting state 
	SD.chdir("/NW");  //Move into northern widget folder from root
	SD.chdir(SN);  //Move into specific numbered sub folder
	SD.chdir("Logs"); //Move into the logs sub-folder
	File DataFile = SD.open(FileNameC, FILE_WRITE);

	// if the file is available, write to it:
	if (DataFile) {
		DataFile.println(Val);
	   // return 0;
	}
	// if the file isn't open, pop up an error:
	else {
	   // return -1;
	}

	DataFile.close();
}

void Margay::LED_Color(unsigned long Val) //Set color of onboard led
{
	int Red = 0; //Red led color
	int Green = 0;  //Green led color
	int Blue = 0;  //Blue led color
	int Lum = 0;  //Luminosity

	//Parse all values from single Val
	Blue = Val & 0xFF;
	Green = (Val >> 8) & 0xFF;
	Red = (Val >> 16) & 0xFF;
	Lum = (Val >> 24) & 0xFF;
	//  Lum = 255 - Lum; //Invert since LEDs are open drain

	analogWrite(RedLED, 255 - (Red * Lum)/0xFF);
	analogWrite(GreenLED, 255 - (Green * Lum)/0xFF);
	analogWrite(BlueLED, 255 - (Blue * Lum)/0xFF);
}

void Margay::GetTime() 
{
	//Update global time string
	// DateTime TimeStamp = RTC.now();
	// LogTimeDate = String(TimeStamp.year()) + "/" + String(TimeStamp.month()) + "/" + String(TimeStamp.day()) + " " + String(TimeStamp.hour()) + ":" + String(TimeStamp.minute()) + ":" + String(TimeStamp.second());  
	LogTimeDate = RTC.getTime(0);
}

float Margay::GetTemp(temp_val Val)
{
	float Vcc = 3.3;
	if(Val == Therm_Val) {
		float Val = float(analogRead(ThermSense_Pin))*(Vcc/1024.0);
		float TempData = TempConvert(Val, Vcc, 10000.0, A, B, C, D, 10000.0);
		TempData = TempData - 273.15; //Get temp from on board thermistor 
		return TempData;
	}
	if(Val == RTC_Val) {
		float RTCTemp = RTC.getTemp();  //Get Temp from RTC
		return RTCTemp;
	}
}

float Margay::GetBatVoltage()
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

float Margay::GetBatPer()
{
	//NOTE: Fit developed for Duracell AA, should work well for most alkalines, but no gaurentee given on accuracy
	//From 305 to 100% capacity, should be accurate to within 1% (for data taken at 25C)
	float A = -1.9809;
	float B = 6.2931;
	float C = -4.0063;
	float Val = GetBatVoltage()/3.0; //Divide to get cell voltage
	float Per = ((A*pow(Val, 2) + B*Val + C)*2 - 1)*100.0; //Return percentage of remaining battery energy
	if(Per < 0) return 0;  //Do not allow return of non-sensical values 
	if(Per > 100) return 100;  //Is this appropriate? Float voltage could be higher than specified and still be correct
	return Per;
}

String Margay::GetOnBoardVals() 
{
	//Get onboard temp, RTC temp, and battery voltage, referance voltage
	// float VRef = analogRead(VRef_Pin);
	float Vcc = 3.3; //(1.8/VRef)*3.3; //Compensate for Vcc using VRef
	// Serial.println(Vcc); //DEBUG!
	float TempData = 0; //FIX!!! Dumb! 

	if(Model < Model_2v0) {  //For older thermistor models 
		float Val = float(analogRead(ThermSense_Pin));
		float Comp = (1.8/3.3)*1024.0/analogRead(VRef_Pin);  //Find compensation value with VRef due to Vcc error
		if(Model == 0) Comp = 1.0; //Overide comp calculation since many v0.0 models do not have ref equiped 
		Val = Val*Comp*(Vcc/1024.0); //Compensate for ref voltage error
		//  float Vout = Vcc - Val;
		//  Serial.println(Val); //DEBUG!
		//  Serial.println(Vout);  //DEBUG!
		TempData = TempConvert(Val, Vcc*Comp, 10000.0, A, B, C, D, 10000.0);
		TempData = TempData - 273.15; //Get temp from on board thermistor 
	}

	// delay(10);
	float BatVoltage = GetBatVoltage(); //Get battery voltage, Include voltage divider in math

	// Temp[3] = Clock.getTemperature(); //Get tempreture from RTC //FIX!
	float RTCTemp = RTC.getTemp();  //Get Temp from RTC
	GetTime(); //FIX!
	if(Model< Model_2v0) return LogTimeDate + "," + String(TempData) + "," + String(RTCTemp) + "," + String(BatVoltage) + ",";
	else return LogTimeDate + "," + String(EnviroSense.GetString()) + String(RTCTemp) + "," + String(BatVoltage) + ",";
}

float Margay::TempConvert(float V, float Vcc, float R, float A, float B, float C, float D, float R25)
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

void Margay::Blink() 
{  
  for(int i = 0; i < 5; i++) {
    digitalWrite(BlueLED, LOW);
    delay(500);
    digitalWrite(BlueLED, HIGH);
    delay(500);
  }
}

float Margay::GetVoltage()  //Get voltage from Ax pin
{
	float Val = adc.GetVoltage();
	return Val;
}

void Margay::Run(String (*Update)(void), unsigned long LogInterval) //Pass in function which returns string of data
{
	// Serial.println("BANG!"); //DEBUG!
	// Serial.println(millis()); //DEBUG!
	if(NewLog) {
		// Serial.println("Log Started!"); //DEBUG
		// LogEvent = true;
		// unsigned long TempLogInterval = LogInterval; //ANDY, Fix with addition of function??
		RTC.setAlarm(LogInterval); //DEBUG!
		InitLogFile(); //Start a new file each time log button is pressed

		//Add inital data point 
		AddDataPoint(Update);
		NewLog = false;  //Clear flag once log is started 
    	Blink();  //Alert user to start of log
    	ResetWD(); //Clear alarm
	}

	if(LogEvent) {
		// Serial.println("Log!"); //DEBUG!
		// RTC.setAlarm(LogInterval);  //Set/reset alarm //DEBUG!
		AddDataPoint(Update); //Write values to SD
		LogEvent = false; //Clear log flag
		// Serial.println("BANG!"); //DEBUG!
		RTC.setAlarm(LogInterval);  //Set/reset alarm
		// Serial.println("ResetTimer"); //DEBUG!
		ResetWD(); //Clear alarm
	}

	if(ManualLog) {  //Write data to SD card without interrupting existing timing cycle
		// Serial.println("Click!"); //DEBUG!
		AddDataPoint(Update); //write values to SD
		ManualLog = false; //Clear log flag
		ResetWD(); //Clear alarm
	}

	if(ExtIntTripped) {  // Defaults to just counter for now
		// Serial.println("TIP!"); //DEBUG!
    ExtInt_count ++;
		ExtIntTripped = false; // Clear interrupt flag flag
		ResetWD(); //Clear alarm
		delay(150); //Hard-code for now; tipping bucket "debounce"
    attachInterrupt(digitalPinToInterrupt(ExtIntPin), Margay::isr2, FALLING);
	}

	if(!digitalRead(RTCInt)) {  //Catch alarm if not reset properly 
   		Serial.println("Reset Alarm"); //DEBUG!
		RTC.setAlarm(LogInterval); //Turn alarm back on 
	}

	AwakeCount++;

	if(AwakeCount > 5) {
	//    AwakeCount = 0;
		// Serial.println(millis()); //DEBUG!
		sleepNow();
	}
	delay(1);
}

void Margay::ResetWD()  //Send a pulse to "feed" the watchdog timer
{
	digitalWrite(WDHold, HIGH); //Set DONE pin high
	delayMicroseconds(5); //Wait a short pulse
	digitalWrite(WDHold, LOW); 
}

void Margay::AddDataPoint(String (*Update)(void)) //Reads new data and writes data to SD
{
	String Data = "";
	if(Model >= Model_2v0) EnviroSense.begin(0x77); //Re-initialize BME280  //FIX??
	// Serial.println("Called Update"); //DEBUG!
	Data = (*Update)(); //Run external update function
	// Serial.println("Request OB Vals"); //DEBUG!
	Data = GetOnBoardVals() + Data; //Append on board readings
	// Serial.println("Got OB Vals");  //DEBUG!
	LogStr(Data);
	// Serial.println("Loged Data"); //DEBUG!
}

//ISRs

void Margay::ButtonLog() 
{
	//ISR to respond to pressing log button and waking device from sleep and starting log
	ManualLog = true; //Set flag to manually record an additional data point
}

void Margay::ExtIntCounter()
{
  // ISR for an external event waking the logger
  detachInterrupt(digitalPinToInterrupt(ExtIntPin));
  ExtIntTripped = true; // Set flag to just increment the counter and return to sleep
}

void Margay::Log() 
{
	//Write global Data to SD
	LogEvent = true; //Set flag for a log event
	AwakeCount = 0; 
}

// ExtInt functions
void Margay::SetExtInt(uint8_t n, String header_entry)
{
  ExtIntPin = n;
  ext_int_header_entry = header_entry;
}

uint16_t Margay::GetExtIntCount(bool reset0)
{
  uint16_t out = ExtInt_count;
  if (reset0)
  {
    ResetExtIntCount(0);
  }
  return out;
}

void Margay::ResetExtIntCount(uint16_t start)
{
    ExtInt_count = start;
}

void Margay::PowerAux(bool State)
{
	pinMode(Ext3v3Ctrl, OUTPUT); //Setup outputs for robustness
	if(State) PowerOB(ON); //Turn on on-board power if required
	if(Model >= Model_2v0) {  //use positive logic for Model v2.0 and newer
		digitalWrite(Ext3v3Ctrl, State); //Switch 3v3 Aux power
	}
	else digitalWrite(Ext3v3Ctrl, !State); //Switch 3v3 Aux power
}

void Margay::PowerOB(bool State)
{
	pinMode(BatSwitch, OUTPUT);
	digitalWrite(BatSwitch, State); //Set bat switch for onboard 3v3/main power
}

void Margay::DateTimeSD(uint16_t* date, uint16_t* time) 
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

void Margay::DateTimeSD_Glob(uint16_t* date, uint16_t* time) {selfPointer->DateTimeSD(date, time);}  //Fix dumb name!

void Margay::isr0() { selfPointer->ButtonLog(); }

// ISR(PCINT0_vect)
// {
// 	ManualLog = true;
// }

// ISR(PCINT0_vect) {
void Margay::isr1() { selfPointer->Log(); }
void Margay::isr2() { selfPointer->ExtIntCounter(); }

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
PowerAux(OFF); //turn off external 3v3 rail
PowerOB(OFF); //Turn off battery connection to sense divider 
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
PowerOB(ON); //Turn on battery connection to sense divider  
PowerAux(ON); //turn on external 3v3 rail
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
