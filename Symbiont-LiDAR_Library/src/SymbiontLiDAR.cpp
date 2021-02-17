#include "SymbiontLiDAR.h"

SymbiontLiDAR::SymbiontLiDAR()
{
}

void SymbiontLiDAR::begin(uint8_t ADR_, uint8_t Sensitivity)
{
    // Set address and sensitivity
    ADR = ADR_;
    Wire.begin();
    Wire.beginTransmission(ADR_);
    Wire.write(0x01);
    Wire.write(Sensitivity);
    Wire.endTransmission();
}

bool SymbiontLiDAR::updateMeasurements(){

    // Time for cap to settle or something
    delay(200);

    // Some of Bobby's unimplemented code to time out if a reading is not taken
    //const unsigned long timeout = 200; //Wait up to 200ms for a new reading
    //unsigned long localTime = millis(); //Keep track of local time for timeout

    // Temporary values and arrays to hold data during I2C parsing.
    // Initialize at 0 as default failure value
    // Perhaps we should reset these to 0 within the loop?
    uint8_t Data1 = 0;
    uint8_t Data2 = 0;
    int16_t DataSet[7];
    
    for(int i = 0; i < 7; i++) {
          Wire.beginTransmission(ADR);
          Wire.write(2*i + 2);
          Wire.endTransmission();
          Wire.requestFrom(ADR, 1);
          Data1 = Wire.read();

          Wire.beginTransmission(ADR);
          Wire.write(2*i + 3);
          Wire.endTransmission();
          Wire.requestFrom(ADR, 1);
          Data2 = Wire.read();
          
          // Store data in array
          // Could save memory by replacing this with if/else
          // for different individual variables
          DataSet[i] = ((Data2 << 8) | Data1);
    }

    // Takes up memory but makes code easier to read. Could be more efficient.
    // Perhaps use pointers, or just comments?
  
    // Measured laser range in centimeters
    Range = DataSet[0];

    //Convert g vals to floats for math
    float gX = DataSet[1];
    float gY = DataSet[2];
    float gZ = DataSet[3];

    float OffsetX = DataSet[4];
    float OffsetY = DataSet[5];
    float OffsetZ = DataSet[6];

    // Calculate roll/pitch and parse errors
    // Error return value = -9999.

    // Errors in range if <0
    if (Range < 0) {
        Range = -9999;
    }

    // Errors in g if all -1
    if(gX == gY && gX == gZ && gX == -1) {  
        Pitch = -9999;
        Roll = -9999;
    }
    // If no errors, continue!
    // Orientation: roll and pitch
    else if( OffsetX == OffsetY && OffsetX == OffsetZ && OffsetX == 0 ) {
        // Do not include angle calc for offset when offset is 0
        // (Results in NAN value)
        Pitch = atan(-gX/gZ)*180./3.14;
        Roll = atan( gY / (sqrt( pow(gX, 2) + pow(gZ, 2) )) )*180./3.14;
    }
    else {
        Pitch = ( atan(-gX/gZ) - atan(-OffsetX/OffsetZ) ) * 180./3.14;
        Roll = ( atan(gY / (sqrt( pow(gX, 2) + pow(gZ, 2) )) ) - \
                 - atan(OffsetY \
                         / (sqrt( pow(OffsetX, 2) + pow(OffsetZ, 2) )) ) ) \
                 * 180./3.14;
    }
    
    if ( (Range == -9999) || (Roll == -9999) || (Pitch == -9999) ){
        return false;
    }
    else {
        return true;
    }
}

float SymbiontLiDAR::getRoll() {
    // Roll in degrees
    return Roll;
}

float SymbiontLiDAR::getPitch() {
    // Pitch in degrees
    return Pitch;
}

float SymbiontLiDAR::getRange() {
    // Distance in cm
    return Range;
}

String SymbiontLiDAR::getString(bool takeNewReadings) {
    if (takeNewReadings) {
        updateMeasurements();
    }
    return String(Range) + "," + String(Pitch) + "," + String(Roll) + ",";
}

String SymbiontLiDAR::getHeader() {
    return "Range [cm],Pitch [deg],Roll [deg],";
}

// Although Arduino standards are for functions to be camelCase, there is a 
// legacy of CapitalCase. Therefore, to not break backwards compatibility:

String SymbiontLiDAR::GetString(bool takeNewReadings) {
    return getString(takeNewReadings);
}

String SymbiontLiDAR::GetHeader() {
    return getHeader();
}

