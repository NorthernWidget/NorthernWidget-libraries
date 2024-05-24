# Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[`Maxbotix`](#classMaxbotix) | Arduino library to enable control and communication with MaxBotix ultrasonic rangefinders using a software serial implementation.

# class `Maxbotix` 

Arduino library to enable control and communication with MaxBotix ultrasonic rangefinders using a software serial implementation.

This Arduino library will enable the microcontroller to control and read output from a MaxBotix ultrasonic rangefinder. It works using software serial, and is not programmed to interface wtih the pulse-width or analog-voltage outputs of the [Maxbotix](#classMaxbotix) rangefinder. It can average multiple measurements to obtain a more robust distance to its target.

* [![DOI](https://zenodo.org/badge/141178473.svg)](https://zenodo.org/badge/latestdoi/141178473)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  `[`Maxbotix`](#classMaxbotix_1a3bf8da390ca20853194470ebb4065bb3)`(uint8_t DataPin)` | 
`public bool `[`begin`](#classMaxbotix_1a8f759400a4aa53f15de30c0b4c5bcd7a)`(uint8_t _nPings,bool _writeAll,uint8_t _ExPin)` | Sets globals and initializes software serial.
`public int16_t `[`getRange`](#classMaxbotix_1ab880391816ebcbba932b68b744cf7857)`()` | Returns the result of a single range measurement  now set for multiple?
`public String `[`getHeader`](#classMaxbotix_1ad27416c96469f44acb46fe7995015d71)`()` | Creates the appropritae header for the data file.
`public String `[`getString`](#classMaxbotix_1aa609c6d39d54c64a5649e6e57de52bf4)`()` | Returns the measurement result(s) as a String object.
`public int16_t `[`GetRange`](#classMaxbotix_1abd641b71596644bc6e2d4e41d176dc50)`()` | CapitalCase version of [getRange()](#classMaxbotix_1ab880391816ebcbba932b68b744cf7857) for backwards compatibility.
`public String `[`GetHeader`](#classMaxbotix_1a6cf7a87fcb56b52222fb68739092db12)`()` | CapitalCase version of [getHeader()](#classMaxbotix_1ad27416c96469f44acb46fe7995015d71) for backwards compatibility.
`public String `[`GetString`](#classMaxbotix_1a9fc625d332d08a01cee7dd312fd5d36f)`()` | CapitalCase version of [getString()](#classMaxbotix_1aa609c6d39d54c64a5649e6e57de52bf4) for backwards compatibility.

## Members

#### `public  `[`Maxbotix`](#classMaxbotix_1a3bf8da390ca20853194470ebb4065bb3)`(uint8_t DataPin)` 

#### `public bool `[`begin`](#classMaxbotix_1a8f759400a4aa53f15de30c0b4c5bcd7a)`(uint8_t _nPings,bool _writeAll,uint8_t _ExPin)` 

Sets globals and initializes software serial.

Sets global variables required for a SoftwareSerial interface to record data from a MaxBotix ultrasonic rangefinder. Initializes software serial based on _RxPin.

#### Parameters
* `_RxPin` Pin for SoftwareSerial receive at 9600 bps.

* `_npings` Number of pings over which you average; each ping itself includes ten short readings that the sensor internally processes. Must be > 0, obviously, and <= 255. Defaults to "1"

* `_writeAll` will write each reading of the sensor (each ping) to the serial monitor and SD card. This is relevant only if npings > 1

* `_ExPin` Excitation pin that turns the sensor on; defaults to "-1", assuming that the sensor is either always on or is being switched via its main power supply.

* `_RS232` defaults false for standard (TTL) logic; true for inverse (RS232-style) logic. It works at standard logger voltages: this is not true RS232, but this is what MaxBotix calls it.

* `_minRange_mm` Minimum sensor range in mm; defaults to 501

* `_maxRange_mm` Maximum sensor range in mm; defaults to 4999

Example: 
```cpp
// SoftwareSerial with RxPin 7, averaging over 10 pings, and otherwise
// using default settings
alog.maxbotixHRXL_WR_Serial(7, 10);
```

#### `public int16_t `[`getRange`](#classMaxbotix_1ab880391816ebcbba932b68b744cf7857)`()` 

Returns the result of a single range measurement  now set for multiple?

Returns the result of a single range measurement. Communications error value = -9999 Internal error value (including range too long) = 5000 Range too short error value = 500 This code makes these internal error values negative to more easily sort them out of the real results

#### `public String `[`getHeader`](#classMaxbotix_1ad27416c96469f44acb46fe7995015d71)`()` 

Creates the appropritae header for the data file.

Creates the appropritae header for the data file, based on provided inputs (number of pings, recording all pings)

#### `public String `[`getString`](#classMaxbotix_1aa609c6d39d54c64a5649e6e57de52bf4)`()` 

Returns the measurement result(s) as a String object.

Returns the measurement result(s) as a String object

#### `public int16_t `[`GetRange`](#classMaxbotix_1abd641b71596644bc6e2d4e41d176dc50)`()` 

CapitalCase version of [getRange()](#classMaxbotix_1ab880391816ebcbba932b68b744cf7857) for backwards compatibility.

#### `public String `[`GetHeader`](#classMaxbotix_1a6cf7a87fcb56b52222fb68739092db12)`()` 

CapitalCase version of [getHeader()](#classMaxbotix_1ad27416c96469f44acb46fe7995015d71) for backwards compatibility.

#### `public String `[`GetString`](#classMaxbotix_1a9fc625d332d08a01cee7dd312fd5d36f)`()` 

CapitalCase version of [getString()](#classMaxbotix_1aa609c6d39d54c64a5649e6e57de52bf4) for backwards compatibility.

Generated by [Moxygen](https://sourcey.com/moxygen)