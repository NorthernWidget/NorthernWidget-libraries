# Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[`Resnik`](#classResnik) | An Arduino-compatible library for utilizing the basic and logging features of the [Resnik](#classResnik) data logger.

# class `Resnik` 

An Arduino-compatible library for utilizing the basic and logging features of the [Resnik](#classResnik) data logger.

Data-logger management. Basic operations, power management, on-board sensing, and links to external devices.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public int `[`SD_CS`](#classResnik_1af102353dfc4ce29ca47bca88a241d067) | SD card chip select **Pin 4**
`public uint8_t `[`BuiltInLED`](#classResnik_1ab5b70f465de1b94507124c583bb84414) | Pin controlling on/off state of RGB LED **Pin 20**
`public uint8_t `[`RedLED`](#classResnik_1a74d98113171004b6eb87537a20141eed) | Pin controlling red intensity of RGB LED **Pin 13**
`public uint8_t `[`GreenLED`](#classResnik_1afc1f6a1666214dfef8fa3b6aac461b87) | Pin controlling green intensity of RGB LED **Pin 15**
`public uint8_t `[`BlueLED`](#classResnik_1a73a6f14eee45df33877bc85f4df8eaa6) | Pin controlling blue intensity of RGB LED **Pin 14**
`public uint8_t `[`SD_CD`](#classResnik_1ab21f0ec15f4edaa810ca863e503f40a2) | Chip-detect pin for the SD card **Pin 1**
`public uint8_t `[`I2C_SW`](#classResnik_1aecce001b2dbd207df6b5f192ac328bef) | Switch to enable/disable I2C **Pin 21**
`public uint8_t `[`RTCInt`](#classResnik_1ae8558f0cacd66ee8ad8c8eb1d1c6700a) | Interrupt from RTC **Pin 10*****DIFFERENT IN CPP!!!!***
`public uint8_t `[`LogInt`](#classResnik_1aaf1fe707a0ac0d089d5b049487eaf535) | Interrupt from Log button **Pin 2*****DIFFERENT IN CPP!!!!***
`public uint8_t `[`TX`](#classResnik_1a053048d40a6a455214ef015fe6903f75) | USART Transmit.
`public uint8_t `[`RX`](#classResnik_1a4fa68e4d56bb53d7c3759537fc9c6fd9) | USART Receive.
`public uint8_t `[`C0`](#classResnik_1ad4543f4279389389e58cdfe1a9abd3f1) | BSCHULZ1701: WHAT ARE THESE??
`public uint8_t `[`C1`](#classResnik_1a75db707781a4e1c29e79c9143fb7c8d6) | BSCHULZ1701: WHAT ARE THESE??
`public uint8_t `[`Sw_Bus_Prime`](#classResnik_1ad56c76a30559a7248ac2676ac4aea94a) | Primary bus switch pin (BSCHULZ1701: is this in addition to the physical switch?)
`public uint8_t `[`Sw_Bus_Sec`](#classResnik_1a8b9d5991e02ff54f1a7d643c02f393d1) | Secondary bus switch pin (BSCHULZ1701: is this in addition to the physical switch?)
`public uint8_t `[`PG_3v3_Core`](#classResnik_1a24102b6f4ab83b6e38cd803d675495c0) | IO Exp PORT B (BSCHULZ1701: what is this in plain English / purpose?)
`public uint8_t `[`FeatherEN`](#classResnik_1ac024da0c2244a0b16e8ac0034d9bdab2) | IO Exp PORT B (BSCHULZ1701: turn Feather on if True, I guess?)
`public uint8_t `[`D0`](#classResnik_1af47cf97f8ec7dab38f2e9c623c4f2dd9) | GPIO pin D0 **Arduino Pin 12**
`public uint8_t `[`D1`](#classResnik_1a07259e123bb63265f7131a8a0432aaff) | GPIO pin D1 **Arduino Pin 25**
`public uint8_t `[`D2`](#classResnik_1ac39c0fbb5def88c47078807f475a4860) | GPIO pin D2 **Arduino Pin 3**
`public uint8_t `[`D3`](#classResnik_1a77c0a7308e09572bcaa24ab0f6c5c722) | GPIO pin D3 **Arduino Pin 26**
`public uint8_t `[`ADC_Sense_SW`](#classResnik_1a202aa688956bfa15de80bd0602640b0e) | Switch the Analog-Digital Converter on (true) or off (false) **Pin 0**
`public uint8_t `[`FeatherRTS`](#classResnik_1a1450dc4e5c2a746759c3cbe39bad52e9) | Feather pin: should this be public or private?
`public uint8_t `[`FeatherCTS`](#classResnik_1a686d54e1e4a04ee28ec293b3ad40c166) | Feather pin: should this be public or private?
`public uint8_t `[`FeatherGPIO`](#classResnik_1addc44f8765d9010a3b6470d7096ec115) | Feather pin: should this be public or private?
`public uint8_t `[`CS_Ext`](#classResnik_1a9afcdd06ced973c2e912c7ef4f956cb6) | Feather pin: should this be public or private?
`public uint8_t `[`GlobalInt`](#classResnik_1a2aaf4bf0c1e91893d54bc4a499ec66c6) | WHAT IS THIS?
`public const String `[`LibVersion`](#classResnik_1ae5da3bcaa2a4dabbec25a3f8ba806622) | [Resnik](#classResnik) data logger library version.
`public  `[`Resnik`](#classResnik_1a6a526860c715639ebc617fa930551fa0)`(board Model_,build Specs_)` | Instantiate the [Resnik](#classResnik) data-logger class.
`public int `[`begin`](#classResnik_1a3796307655e11534185877592c3d61a4)`(uint8_t * Vals,uint8_t NumVals,String Header_)` | Begin with a list of attached I2C devices.
`public int `[`begin`](#classResnik_1a5c8e3264d3f60e04895ea1d02c646e2b)`(String Header_)` | Begin by passing a header string; default empty.
`public int `[`LogStr`](#classResnik_1a1af4e62ae9eff50d164895f8e313d42f)`(String Val)` | Write a string to the log file.
`public String `[`ReadStr`](#classResnik_1a284c8fbb39143e0d6ae97176c0c16f3a)`(uint8_t LineIndex,uint32_t DataIndex)` | BSCHULZ1701: WHAT EXACTLY DOES THIS FUNCTION DO?
`public void `[`LED_Color`](#classResnik_1a2992e3a02a980b86dea20459e193eed3)`(unsigned long Val)` | Set the color of the LED.
`public void `[`Run`](#classResnik_1ae34a94b97b007b978c567a6c31fe55fb)`(String(*)(void) Update,unsigned long LogInterval)` | Main function that loops infinititely during runtime BSCHULZ1701: I changed *f to *Update to match the CPP. Hope that does not break anything!
`public float `[`GetVoltage`](#classResnik_1ab0434d03a33299fa0485b0b2821c98e4)`(uint8_t Pin)` | Read voltage from the external 16-bit ADC.
`public uint8_t `[`SetVoltageRaw`](#classResnik_1acb2991acfaea8c4ed4fa99f1401ff900)`(uint16_t Val,bool Gain)` | Set voltage on the 12-bit digital-to-analog converter.
`public uint8_t `[`SetVoltage`](#classResnik_1ab0ecb7fa5993cc834f675c124dd1eab3)`(float Val)` | Set voltage on the 12-bit digital-to-analog converter.
`public void `[`AddDataPoint`](#classResnik_1ab23092611c58905c74e419f89eaeb284)`(String(*)(void) Update)` | Writes date/time, on-board sensors, and external string to SD.
`public String `[`GetOnBoardVals`](#classResnik_1a24bc9bf3345104126008453c22395861)`()` | Obtain date/time, P/T/RH, temperatures, and voltages from board.
`public void `[`InitLogFile`](#classResnik_1ac871946ed5abf58ea411da8db0ed8a64)`()` | Create a new log file, following serial numbering.
`public uint8_t `[`PowerAuto`](#classResnik_1af84c7ed29a06b338b2fd0f54f6d8c047)`()` | Determine which input has power and set up power path from that.
`public void `[`PowerAux`](#classResnik_1a53f60b44d8c2a83ad962ecd15c54b2f1)`(uint8_t State)` | Power from Main, backup, or off BSCHULZ1701: I am not sure what this does!
`public void `[`I2CState`](#classResnik_1a3c5ffdc60c6e7ea45f905ad5c48196a3)`(bool State)` | Set the I2C state.

## Members

#### `public int `[`SD_CS`](#classResnik_1af102353dfc4ce29ca47bca88a241d067) 

SD card chip select **Pin 4**

#### `public uint8_t `[`BuiltInLED`](#classResnik_1ab5b70f465de1b94507124c583bb84414) 

Pin controlling on/off state of RGB LED **Pin 20**

#### `public uint8_t `[`RedLED`](#classResnik_1a74d98113171004b6eb87537a20141eed) 

Pin controlling red intensity of RGB LED **Pin 13**

#### `public uint8_t `[`GreenLED`](#classResnik_1afc1f6a1666214dfef8fa3b6aac461b87) 

Pin controlling green intensity of RGB LED **Pin 15**

#### `public uint8_t `[`BlueLED`](#classResnik_1a73a6f14eee45df33877bc85f4df8eaa6) 

Pin controlling blue intensity of RGB LED **Pin 14**

#### `public uint8_t `[`SD_CD`](#classResnik_1ab21f0ec15f4edaa810ca863e503f40a2) 

Chip-detect pin for the SD card **Pin 1**

#### `public uint8_t `[`I2C_SW`](#classResnik_1aecce001b2dbd207df6b5f192ac328bef) 

Switch to enable/disable I2C **Pin 21**

#### `public uint8_t `[`RTCInt`](#classResnik_1ae8558f0cacd66ee8ad8c8eb1d1c6700a) 

Interrupt from RTC **Pin 10*****DIFFERENT IN CPP!!!!***

#### `public uint8_t `[`LogInt`](#classResnik_1aaf1fe707a0ac0d089d5b049487eaf535) 

Interrupt from Log button **Pin 2*****DIFFERENT IN CPP!!!!***

#### `public uint8_t `[`TX`](#classResnik_1a053048d40a6a455214ef015fe6903f75) 

USART Transmit.

#### `public uint8_t `[`RX`](#classResnik_1a4fa68e4d56bb53d7c3759537fc9c6fd9) 

USART Receive.

#### `public uint8_t `[`C0`](#classResnik_1ad4543f4279389389e58cdfe1a9abd3f1) 

BSCHULZ1701: WHAT ARE THESE??

#### `public uint8_t `[`C1`](#classResnik_1a75db707781a4e1c29e79c9143fb7c8d6) 

BSCHULZ1701: WHAT ARE THESE??

#### `public uint8_t `[`Sw_Bus_Prime`](#classResnik_1ad56c76a30559a7248ac2676ac4aea94a) 

Primary bus switch pin (BSCHULZ1701: is this in addition to the physical switch?)

#### `public uint8_t `[`Sw_Bus_Sec`](#classResnik_1a8b9d5991e02ff54f1a7d643c02f393d1) 

Secondary bus switch pin (BSCHULZ1701: is this in addition to the physical switch?)

#### `public uint8_t `[`PG_3v3_Core`](#classResnik_1a24102b6f4ab83b6e38cd803d675495c0) 

IO Exp PORT B (BSCHULZ1701: what is this in plain English / purpose?)

#### `public uint8_t `[`FeatherEN`](#classResnik_1ac024da0c2244a0b16e8ac0034d9bdab2) 

IO Exp PORT B (BSCHULZ1701: turn Feather on if True, I guess?)

#### `public uint8_t `[`D0`](#classResnik_1af47cf97f8ec7dab38f2e9c623c4f2dd9) 

GPIO pin D0 **Arduino Pin 12**

#### `public uint8_t `[`D1`](#classResnik_1a07259e123bb63265f7131a8a0432aaff) 

GPIO pin D1 **Arduino Pin 25**

#### `public uint8_t `[`D2`](#classResnik_1ac39c0fbb5def88c47078807f475a4860) 

GPIO pin D2 **Arduino Pin 3**

#### `public uint8_t `[`D3`](#classResnik_1a77c0a7308e09572bcaa24ab0f6c5c722) 

GPIO pin D3 **Arduino Pin 26**

#### `public uint8_t `[`ADC_Sense_SW`](#classResnik_1a202aa688956bfa15de80bd0602640b0e) 

Switch the Analog-Digital Converter on (true) or off (false) **Pin 0**

#### `public uint8_t `[`FeatherRTS`](#classResnik_1a1450dc4e5c2a746759c3cbe39bad52e9) 

Feather pin: should this be public or private?

#### `public uint8_t `[`FeatherCTS`](#classResnik_1a686d54e1e4a04ee28ec293b3ad40c166) 

Feather pin: should this be public or private?

#### `public uint8_t `[`FeatherGPIO`](#classResnik_1addc44f8765d9010a3b6470d7096ec115) 

Feather pin: should this be public or private?

#### `public uint8_t `[`CS_Ext`](#classResnik_1a9afcdd06ced973c2e912c7ef4f956cb6) 

Feather pin: should this be public or private?

#### `public uint8_t `[`GlobalInt`](#classResnik_1a2aaf4bf0c1e91893d54bc4a499ec66c6) 

WHAT IS THIS?

#### `public const String `[`LibVersion`](#classResnik_1ae5da3bcaa2a4dabbec25a3f8ba806622) 

[Resnik](#classResnik) data logger library version.

#### `public  `[`Resnik`](#classResnik_1a6a526860c715639ebc617fa930551fa0)`(board Model_,build Specs_)` 

Instantiate the [Resnik](#classResnik) data-logger class.

#### Parameters
* `Model_` Not currently used for [Resnik](#classResnik)

* `Specs_` Defines the number of on-board I2C device addresses. **Build_A**: **DEFAULT** 6 on-board I2C devices. **Build_B**: 0 on-board I2C devices.

#### `public int `[`begin`](#classResnik_1a3796307655e11534185877592c3d61a4)`(uint8_t * Vals,uint8_t NumVals,String Header_)` 

Begin with a list of attached I2C devices.

#### Parameters
* `*Vals` List of I2C addresses for external sensors 

* `NumVals` The length of the *Vals list 

* `Header_` A header string for the data file

#### `public int `[`begin`](#classResnik_1a5c8e3264d3f60e04895ea1d02c646e2b)`(String Header_)` 

Begin by passing a header string; default empty.

#### Parameters
* `Header_` A header string for the data file

#### `public int `[`LogStr`](#classResnik_1a1af4e62ae9eff50d164895f8e313d42f)`(String Val)` 

Write a string to the log file.

#### Parameters
* `Val` The string to be written

#### `public String `[`ReadStr`](#classResnik_1a284c8fbb39143e0d6ae97176c0c16f3a)`(uint8_t LineIndex,uint32_t DataIndex)` 

BSCHULZ1701: WHAT EXACTLY DOES THIS FUNCTION DO?

#### Parameters
* `LineIndex` The desired line number 

* `DataIndex` The desired byte at which to start in the file

#### `public void `[`LED_Color`](#classResnik_1a2992e3a02a980b86dea20459e193eed3)`(unsigned long Val)` 

Set the color of the LED.

#### Parameters
* `Val` 4-byte (R, G, B, alpha) color

#### `public void `[`Run`](#classResnik_1ae34a94b97b007b978c567a6c31fe55fb)`(String(*)(void) Update,unsigned long LogInterval)` 

Main function that loops infinititely during runtime BSCHULZ1701: I changed *f to *Update to match the CPP. Hope that does not break anything!

#### Parameters
* `*Update` Pointer to a function in the Arduino sketch (ino); this retrieves new sensor data as a String object 

* `LogInterval` Number of seconds between log events

#### `public float `[`GetVoltage`](#classResnik_1ab0434d03a33299fa0485b0b2821c98e4)`(uint8_t Pin)` 

Read voltage from the external 16-bit ADC.

#### Parameters
* `Pin` (range 0-3)  which pin to read?

#### `public uint8_t `[`SetVoltageRaw`](#classResnik_1acb2991acfaea8c4ed4fa99f1401ff900)`(uint16_t Val,bool Gain)` 

Set voltage on the 12-bit digital-to-analog converter.

#### Parameters
* `Val` 0 to 4095, which scales from 0 to Vbus 

* `Gain` WHAT? WHY BOOL?

BSCHULZ1701: WHY IS THE GAIN A BOOLEAN? AND IS THIS FROM 0 TO VBUS? AND SHOULD THIS BE A PRIVATE UTILITY FUNCTION?

#### `public uint8_t `[`SetVoltage`](#classResnik_1ab0ecb7fa5993cc834f675c124dd1eab3)`(float Val)` 

Set voltage on the 12-bit digital-to-analog converter.

#### Parameters
* `Val` Desired voltage value, in volts.

#### `public void `[`AddDataPoint`](#classResnik_1ab23092611c58905c74e419f89eaeb284)`(String(*)(void) Update)` 

Writes date/time, on-board sensors, and external string to SD.

#### Parameters
* `*Update` External update function from Arduino sketch

#### `public String `[`GetOnBoardVals`](#classResnik_1a24bc9bf3345104126008453c22395861)`()` 

Obtain date/time, P/T/RH, temperatures, and voltages from board.

#### `public void `[`InitLogFile`](#classResnik_1ac871946ed5abf58ea411da8db0ed8a64)`()` 

Create a new log file, following serial numbering.

#### `public uint8_t `[`PowerAuto`](#classResnik_1af84c7ed29a06b338b2fd0f54f6d8c047)`()` 

Determine which input has power and set up power path from that.

#### `public void `[`PowerAux`](#classResnik_1a53f60b44d8c2a83ad962ecd15c54b2f1)`(uint8_t State)` 

Power from Main, backup, or off BSCHULZ1701: I am not sure what this does!

#### Parameters
* `State`

#### `public void `[`I2CState`](#classResnik_1a3c5ffdc60c6e7ea45f905ad5c48196a3)`(bool State)` 

Set the I2C state.

#### Parameters
* `State` True or False gives On or Off

Generated by [Moxygen](https://sourcey.com/moxygen)