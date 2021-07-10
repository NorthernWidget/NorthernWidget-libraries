# Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[`Okapi`](#classOkapi) | An Arduino-compatible library for utilizing the basic and logging features of the [Okapi](#classOkapi) data logger.

# class `Okapi` 

An Arduino-compatible library for utilizing the basic and logging features of the [Okapi](#classOkapi) data logger.

Data-logger management. Basic operations, power management, on-board sensing, and links to external devices.

[![DOI](https://zenodo.org/badge/197810426.svg)](https://zenodo.org/badge/latestdoi/197810426)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public int `[`SD_CS`](#classOkapi_1ac8aebb36248453d35425df5d9b357e6f) | SD card chip select **Pin 4**
`public uint8_t `[`BuiltInLED`](#classOkapi_1ae956d96528d41fb37b71a5d263f1bdad) | Pin controlling on/off state of RGB LED **Pin 20**
`public uint8_t `[`RedLED`](#classOkapi_1acf32b5ca0d6f411ecb0d62035f3e89e4) | Pin controlling red intensity of RGB LED **Pin 13**
`public uint8_t `[`GreenLED`](#classOkapi_1a69968e64c5a7b286c01e3f2255445ac8) | Pin controlling green intensity of RGB LED **Pin 15**
`public uint8_t `[`BlueLED`](#classOkapi_1a0799a21e7102afc878b15306308bf13c) | Pin controlling blue intensity of RGB LED **Pin 14**
`public uint8_t `[`SD_CD`](#classOkapi_1abe6bb3e83e8b62c32e59278c63043c2a) | Chip-detect pin for the SD card **Pin 1**
`public uint8_t `[`I2C_SW`](#classOkapi_1a823782debad4c13e6864c86bac305252) | Switch to enable/disable I2C **Pin 21**
`public uint8_t `[`RTCInt`](#classOkapi_1a4aeeb6a70a0d9cb97d6841ad4b97e51f) | Interrupt from RTC **Pin 10*****DIFFERENT IN CPP!!!!***
`public uint8_t `[`LogInt`](#classOkapi_1a89b176eadc11b85422e80d3ac7fa4974) | Interrupt from Log button **Pin 2*****DIFFERENT IN CPP!!!!***
`public uint8_t `[`TX`](#classOkapi_1a54d8133829b288fcbd3f78a03fed788a) | USART Transmit.
`public uint8_t `[`RX`](#classOkapi_1a985836f25ba6ef2187acdb9a5b0b42fc) | USART Receive.
`public uint8_t `[`C0`](#classOkapi_1a8be530c7e73a31281a148947181936cf) | BSCHULZ1701: WHAT ARE THESE??
`public uint8_t `[`C1`](#classOkapi_1ae5c1b76fd63487e76108d59b7da59655) | BSCHULZ1701: WHAT ARE THESE??
`public uint8_t `[`Sw_Bus_Prime`](#classOkapi_1abd6b1be6b778dfee5ddd627e951e373c) | Primary bus switch pin (BSCHULZ1701: is this in addition to the physical switch?)
`public uint8_t `[`Sw_Bus_Sec`](#classOkapi_1a20d40de9e964d2a2c295775abc1b3687) | Secondary bus switch pin (BSCHULZ1701: is this in addition to the physical switch?)
`public uint8_t `[`PG_3v3_Core`](#classOkapi_1a3b1c83dc99268390258c86033d901546) | IO Exp PORT B (BSCHULZ1701: what is this in plain English / purpose?)
`public uint8_t `[`FeatherEN`](#classOkapi_1aed2f90edb22ae1410f8c455a0850f38c) | IO Exp PORT B (BSCHULZ1701: turn Feather on if True, I guess?)
`public uint8_t `[`D0`](#classOkapi_1af52d7f037b48338b8050ad10e5744553) | GPIO pin D0 **Arduino Pin 12**
`public uint8_t `[`D1`](#classOkapi_1ad45c3cb1e56f3fe6a4b88b18d0b0f37a) | GPIO pin D1 **Arduino Pin 25**
`public uint8_t `[`D2`](#classOkapi_1ac4fbc3a46b5526898daa73b51a5af86f) | GPIO pin D2 **Arduino Pin 3**
`public uint8_t `[`D3`](#classOkapi_1a5e49dec1425d28902b61b56ee711fa03) | GPIO pin D3 **Arduino Pin 26**
`public uint8_t `[`ADC_Sense_SW`](#classOkapi_1a0a5978462fc3c24f45217d9b6b1c104f) | Switch the Analog-Digital Converter on (true) or off (false) **Pin 0**
`public uint8_t `[`FeatherRTS`](#classOkapi_1a10c8ce6c4d17907badf0a6a68a87a810) | Feather pin: should this be public or private?
`public uint8_t `[`FeatherCTS`](#classOkapi_1ac9a3165eda6ee19f15fce64d697f4caf) | Feather pin: should this be public or private?
`public uint8_t `[`FeatherGPIO`](#classOkapi_1ac5240cee560ada7adb878c4665757ee6) | Feather pin: should this be public or private?
`public uint8_t `[`CS_Ext`](#classOkapi_1a4ab690c904b04819e0bf09e332284257) | Feather pin: should this be public or private?
`public uint8_t `[`GlobalInt`](#classOkapi_1aba6d5dbf2da8675727b57f35c0674ad5) | WHAT IS THIS?
`public const String `[`LibVersion`](#classOkapi_1ad603ff809f486c388780b0d89c6725f8) | [Okapi](#classOkapi) data logger library version.
`public  `[`Okapi`](#classOkapi_1adf82d382ff6e2cba37c6f5beaea7781c)`(board Model_,build Specs_)` | Instantiate the [Okapi](#classOkapi) data-logger class.
`public int `[`begin`](#classOkapi_1aa6f69f7882d3eb7eed9de28d74f091fc)`(uint8_t * Vals,uint8_t NumVals,String Header_)` | Begin with a list of attached I2C devices.
`public int `[`begin`](#classOkapi_1ae178444e3a58d5817b1fad6ea7127d69)`(String Header_)` | Begin by passing a header string; default empty.
`public int `[`LogStr`](#classOkapi_1a3575dd8b8ca6d6f9fb74732e6ab0c38a)`(String Val)` | Write a string to the log file.
`public String `[`ReadStr`](#classOkapi_1a8ced469be081525f42efb458bd431072)`(uint8_t LineIndex,uint32_t DataIndex)` | BSCHULZ1701: WHAT EXACTLY DOES THIS FUNCTION DO?
`public void `[`LED_Color`](#classOkapi_1a38dd97d18c76a6d9fbcffc68e5fc11e7)`(unsigned long Val)` | Set the color of the LED.
`public void `[`Run`](#classOkapi_1ad5f76b5e59aabf99da8327e40470d17c)`(String(*)(void) Update,unsigned long LogInterval)` | Main function that loops infinititely during runtime BSCHULZ1701: I changed *f to *Update to match the CPP. Hope that does not break anything!
`public float `[`GetVoltage`](#classOkapi_1ad00e352c6075a35047b995d32a64637e)`(uint8_t Pin)` | Read voltage from the external 16-bit ADC.
`public uint8_t `[`SetVoltageRaw`](#classOkapi_1ab08565f0549273b8274c9ecf41a482d8)`(uint16_t Val,bool Gain)` | Set voltage on the 12-bit digital-to-analog converter.
`public uint8_t `[`SetVoltage`](#classOkapi_1a07d86f923be462b809378d879cd613c4)`(float Val)` | Set voltage on the 12-bit digital-to-analog converter.
`public void `[`AddDataPoint`](#classOkapi_1aa1630c73c06162a9e06e6a258d1c0311)`(String(*)(void) Update)` | Writes date/time, on-board sensors, and external string to SD.
`public String `[`GetOnBoardVals`](#classOkapi_1a9639a77625a93ed6e886a13eb9b0965a)`()` | Obtain date/time, P/T/RH, temperatures, and voltages from board.
`public void `[`InitLogFile`](#classOkapi_1a33aba68d863d5df33dd990267adc4c44)`()` | Create a new log file, following serial numbering.
`public uint8_t `[`PowerAuto`](#classOkapi_1a8608676f7e32fef3d9c33b6f91accdc6)`()` | Determine which input has power and set up power path from that.
`public void `[`PowerAux`](#classOkapi_1aea0dfb477da9f99fe33a69433dab10dd)`(uint8_t State)` | Power from Main, backup, or off BSCHULZ1701: I am not sure what this does!
`public void `[`I2CState`](#classOkapi_1ab87b9bbce2d763ef124e30e806d4aa4e)`(bool State)` | Set the I2C state.

## Members

#### `public int `[`SD_CS`](#classOkapi_1ac8aebb36248453d35425df5d9b357e6f) 

SD card chip select **Pin 4**

#### `public uint8_t `[`BuiltInLED`](#classOkapi_1ae956d96528d41fb37b71a5d263f1bdad) 

Pin controlling on/off state of RGB LED **Pin 20**

#### `public uint8_t `[`RedLED`](#classOkapi_1acf32b5ca0d6f411ecb0d62035f3e89e4) 

Pin controlling red intensity of RGB LED **Pin 13**

#### `public uint8_t `[`GreenLED`](#classOkapi_1a69968e64c5a7b286c01e3f2255445ac8) 

Pin controlling green intensity of RGB LED **Pin 15**

#### `public uint8_t `[`BlueLED`](#classOkapi_1a0799a21e7102afc878b15306308bf13c) 

Pin controlling blue intensity of RGB LED **Pin 14**

#### `public uint8_t `[`SD_CD`](#classOkapi_1abe6bb3e83e8b62c32e59278c63043c2a) 

Chip-detect pin for the SD card **Pin 1**

#### `public uint8_t `[`I2C_SW`](#classOkapi_1a823782debad4c13e6864c86bac305252) 

Switch to enable/disable I2C **Pin 21**

#### `public uint8_t `[`RTCInt`](#classOkapi_1a4aeeb6a70a0d9cb97d6841ad4b97e51f) 

Interrupt from RTC **Pin 10*****DIFFERENT IN CPP!!!!***

#### `public uint8_t `[`LogInt`](#classOkapi_1a89b176eadc11b85422e80d3ac7fa4974) 

Interrupt from Log button **Pin 2*****DIFFERENT IN CPP!!!!***

#### `public uint8_t `[`TX`](#classOkapi_1a54d8133829b288fcbd3f78a03fed788a) 

USART Transmit.

#### `public uint8_t `[`RX`](#classOkapi_1a985836f25ba6ef2187acdb9a5b0b42fc) 

USART Receive.

#### `public uint8_t `[`C0`](#classOkapi_1a8be530c7e73a31281a148947181936cf) 

BSCHULZ1701: WHAT ARE THESE??

#### `public uint8_t `[`C1`](#classOkapi_1ae5c1b76fd63487e76108d59b7da59655) 

BSCHULZ1701: WHAT ARE THESE??

#### `public uint8_t `[`Sw_Bus_Prime`](#classOkapi_1abd6b1be6b778dfee5ddd627e951e373c) 

Primary bus switch pin (BSCHULZ1701: is this in addition to the physical switch?)

#### `public uint8_t `[`Sw_Bus_Sec`](#classOkapi_1a20d40de9e964d2a2c295775abc1b3687) 

Secondary bus switch pin (BSCHULZ1701: is this in addition to the physical switch?)

#### `public uint8_t `[`PG_3v3_Core`](#classOkapi_1a3b1c83dc99268390258c86033d901546) 

IO Exp PORT B (BSCHULZ1701: what is this in plain English / purpose?)

#### `public uint8_t `[`FeatherEN`](#classOkapi_1aed2f90edb22ae1410f8c455a0850f38c) 

IO Exp PORT B (BSCHULZ1701: turn Feather on if True, I guess?)

#### `public uint8_t `[`D0`](#classOkapi_1af52d7f037b48338b8050ad10e5744553) 

GPIO pin D0 **Arduino Pin 12**

#### `public uint8_t `[`D1`](#classOkapi_1ad45c3cb1e56f3fe6a4b88b18d0b0f37a) 

GPIO pin D1 **Arduino Pin 25**

#### `public uint8_t `[`D2`](#classOkapi_1ac4fbc3a46b5526898daa73b51a5af86f) 

GPIO pin D2 **Arduino Pin 3**

#### `public uint8_t `[`D3`](#classOkapi_1a5e49dec1425d28902b61b56ee711fa03) 

GPIO pin D3 **Arduino Pin 26**

#### `public uint8_t `[`ADC_Sense_SW`](#classOkapi_1a0a5978462fc3c24f45217d9b6b1c104f) 

Switch the Analog-Digital Converter on (true) or off (false) **Pin 0**

#### `public uint8_t `[`FeatherRTS`](#classOkapi_1a10c8ce6c4d17907badf0a6a68a87a810) 

Feather pin: should this be public or private?

#### `public uint8_t `[`FeatherCTS`](#classOkapi_1ac9a3165eda6ee19f15fce64d697f4caf) 

Feather pin: should this be public or private?

#### `public uint8_t `[`FeatherGPIO`](#classOkapi_1ac5240cee560ada7adb878c4665757ee6) 

Feather pin: should this be public or private?

#### `public uint8_t `[`CS_Ext`](#classOkapi_1a4ab690c904b04819e0bf09e332284257) 

Feather pin: should this be public or private?

#### `public uint8_t `[`GlobalInt`](#classOkapi_1aba6d5dbf2da8675727b57f35c0674ad5) 

WHAT IS THIS?

#### `public const String `[`LibVersion`](#classOkapi_1ad603ff809f486c388780b0d89c6725f8) 

[Okapi](#classOkapi) data logger library version.

#### `public  `[`Okapi`](#classOkapi_1adf82d382ff6e2cba37c6f5beaea7781c)`(board Model_,build Specs_)` 

Instantiate the [Okapi](#classOkapi) data-logger class.

#### Parameters
* `Model_` Not currently used for [Okapi](#classOkapi)

* `Specs_` Defines the number of on-board I2C device addresses. **Build_A**: **DEFAULT** 6 on-board I2C devices. **Build_B**: 0 on-board I2C devices.

#### `public int `[`begin`](#classOkapi_1aa6f69f7882d3eb7eed9de28d74f091fc)`(uint8_t * Vals,uint8_t NumVals,String Header_)` 

Begin with a list of attached I2C devices.

#### Parameters
* `*Vals` List of I2C addresses for external sensors 

* `NumVals` The length of the *Vals list 

* `Header_` A header string for the data file

#### `public int `[`begin`](#classOkapi_1ae178444e3a58d5817b1fad6ea7127d69)`(String Header_)` 

Begin by passing a header string; default empty.

#### Parameters
* `Header_` A header string for the data file

#### `public int `[`LogStr`](#classOkapi_1a3575dd8b8ca6d6f9fb74732e6ab0c38a)`(String Val)` 

Write a string to the log file.

#### Parameters
* `Val` The string to be written

#### `public String `[`ReadStr`](#classOkapi_1a8ced469be081525f42efb458bd431072)`(uint8_t LineIndex,uint32_t DataIndex)` 

BSCHULZ1701: WHAT EXACTLY DOES THIS FUNCTION DO?

#### Parameters
* `LineIndex` The desired line number 

* `DataIndex` The desired byte at which to start in the file

#### `public void `[`LED_Color`](#classOkapi_1a38dd97d18c76a6d9fbcffc68e5fc11e7)`(unsigned long Val)` 

Set the color of the LED.

#### Parameters
* `Val` 4-byte (R, G, B, alpha) color

#### `public void `[`Run`](#classOkapi_1ad5f76b5e59aabf99da8327e40470d17c)`(String(*)(void) Update,unsigned long LogInterval)` 

Main function that loops infinititely during runtime BSCHULZ1701: I changed *f to *Update to match the CPP. Hope that does not break anything!

#### Parameters
* `*Update` Pointer to a function in the Arduino sketch (ino); this retrieves new sensor data as a String object 

* `LogInterval` Number of seconds between log events

#### `public float `[`GetVoltage`](#classOkapi_1ad00e352c6075a35047b995d32a64637e)`(uint8_t Pin)` 

Read voltage from the external 16-bit ADC.

#### Parameters
* `Pin` (range 0-3)  which pin to read?

#### `public uint8_t `[`SetVoltageRaw`](#classOkapi_1ab08565f0549273b8274c9ecf41a482d8)`(uint16_t Val,bool Gain)` 

Set voltage on the 12-bit digital-to-analog converter.

#### Parameters
* `Val` 0 to 4095, which scales from 0 to Vbus 

* `Gain` WHAT? WHY BOOL?

BSCHULZ1701: WHY IS THE GAIN A BOOLEAN? AND IS THIS FROM 0 TO VBUS? AND SHOULD THIS BE A PRIVATE UTILITY FUNCTION?

#### `public uint8_t `[`SetVoltage`](#classOkapi_1a07d86f923be462b809378d879cd613c4)`(float Val)` 

Set voltage on the 12-bit digital-to-analog converter.

#### Parameters
* `Val` Desired voltage value, in volts.

#### `public void `[`AddDataPoint`](#classOkapi_1aa1630c73c06162a9e06e6a258d1c0311)`(String(*)(void) Update)` 

Writes date/time, on-board sensors, and external string to SD.

#### Parameters
* `*Update` External update function from Arduino sketch

#### `public String `[`GetOnBoardVals`](#classOkapi_1a9639a77625a93ed6e886a13eb9b0965a)`()` 

Obtain date/time, P/T/RH, temperatures, and voltages from board.

#### `public void `[`InitLogFile`](#classOkapi_1a33aba68d863d5df33dd990267adc4c44)`()` 

Create a new log file, following serial numbering.

#### `public uint8_t `[`PowerAuto`](#classOkapi_1a8608676f7e32fef3d9c33b6f91accdc6)`()` 

Determine which input has power and set up power path from that.

#### `public void `[`PowerAux`](#classOkapi_1aea0dfb477da9f99fe33a69433dab10dd)`(uint8_t State)` 

Power from Main, backup, or off BSCHULZ1701: I am not sure what this does!

#### Parameters
* `State`

#### `public void `[`I2CState`](#classOkapi_1ab87b9bbce2d763ef124e30e806d4aa4e)`(bool State)` 

Set the I2C state.

#### Parameters
* `State` True or False gives On or Off

Generated by [Moxygen](https://sourcey.com/moxygen)