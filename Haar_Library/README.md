# Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[`Haar`](#classHaar) | Library for the rugged temperature, pressure, relative-humidity sensor.

# class `Haar` 

Library for the rugged temperature, pressure, relative-humidity sensor.

Project [Haar](#classHaar), named for the cold fog off the north sea, is a pressure, temperature, and relative-humidity sensor that can withstand full submersion. Bobby Schulz designed it after the consistently-near-0C temperatures and 100% relative humidity of Chimborazo Volcano, Ecuador, claimed the lives of many brave but misguided BME-280 units, who then catistrophically bricked their I2C buses and those of their associated data loggers, thus taking down a large fraction of our hydromet network.

*May their silicon and copper souls join the chorus of the stars.*

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  `[`Haar`](#classHaar_1a0da6a368ea9f434647065dbc356350cf)`()` | Instantiate the [Haar](#classHaar) sensor class.
`public uint8_t `[`begin`](#classHaar_1a80119761d396fae3f7f36dc5ef0f5d63)`(uint8_t ADR_)` | Begin communications with the [Haar](#classHaar) sensor.
`public float `[`getPressure`](#classHaar_1ade237d2fa6aa6be08de2d631f8c3c4ab)`(bool update)` | Return the currently stored pressure [in mBar].
`public float `[`getHumidity`](#classHaar_1afacacfa4b648e9aed0c8b42f3f078246)`(bool update)` | Return the currently stored relative humidity [%].
`public float `[`getTemperature`](#classHaar_1ac0c2a08ab816a6fa66bc1a4e41598956)`(Sensor Device,bool update)` | Return the currently stored Temperature [degrees C].
`public uint8_t `[`sleep`](#classHaar_1afb0d27a90e98565bc4db523574cd781b)`(bool state)` | Enable or disable device sleep mode THIS FUNCTION CURRENTLY DOES NOTHING!
`public uint8_t `[`updateMeasurements`](#classHaar_1aba9328a25d0d424045ae3dbf6edcaa35)`(bool block)` | Return a new sample of all of the data.
`public bool `[`newData`](#classHaar_1ae51c1fcc10010c56263e9df311a83486)`()` | Checks for updated data. Returns `true` if new data are available; otherwise returns `false`
`public String `[`getString`](#classHaar_1a3c36187bfc3d8254ff28446c315e60d7)`()` | The most important function for the user! Returns all data as a comma-separated string: "P,RH,T(P),T[RH],".
`public String `[`getHeader`](#classHaar_1a271e4eb4e28723df81c1a0ed1d2d0ead)`()` | Returns a header: "Pressure Atmos [mBar], Humidity [%], Temp Pres [C], Temp RH [C],".

## Members

#### `public  `[`Haar`](#classHaar_1a0da6a368ea9f434647065dbc356350cf)`()` 

Instantiate the [Haar](#classHaar) sensor class.

#### `public uint8_t `[`begin`](#classHaar_1a80119761d396fae3f7f36dc5ef0f5d63)`(uint8_t ADR_)` 

Begin communications with the [Haar](#classHaar) sensor.

#### Parameters
* `ADR_` I2C address. Defaults to 0x40. DOES NOT ACTUALLY SEEM TO BE USED! 0x40 FOR EVERYTHING, REGARDLESS

#### `public float `[`getPressure`](#classHaar_1ade237d2fa6aa6be08de2d631f8c3c4ab)`(bool update)` 

Return the currently stored pressure [in mBar].

#### Parameters
* `update` Read and store a new pressure value before sending? By default is false; simply returns the already-available pressure value.

#### `public float `[`getHumidity`](#classHaar_1afacacfa4b648e9aed0c8b42f3f078246)`(bool update)` 

Return the currently stored relative humidity [%].

#### Parameters
* `update` Read and store a new pressure value before sending? By default is false; simply returns the already-available RH value.

#### `public float `[`getTemperature`](#classHaar_1ac0c2a08ab816a6fa66bc1a4e41598956)`(Sensor Device,bool update)` 

Return the currently stored Temperature [degrees C].

#### Parameters
* `Device` Selects which sensor is used to measure the temperature. Input can be `RH_Sense`, which corresponds to a 0, or `Pres_Sense`, which corresponds to a 1. By default, this is RH_Sense, as this sensor has a better internal temperature sensor. 

* `update` Read and store a new pressure value before sending? By default is false; simply returns the already-available temperature value.

#### `public uint8_t `[`sleep`](#classHaar_1afb0d27a90e98565bc4db523574cd781b)`(bool state)` 

Enable or disable device sleep mode THIS FUNCTION CURRENTLY DOES NOTHING!

#### Parameters
* `State` Can be `ON` (1) or `OFF` (0). By default, this command sets "Sleep" to ON, sending the device into a low-power sleep mode.

#### `public uint8_t `[`updateMeasurements`](#classHaar_1aba9328a25d0d424045ae3dbf6edcaa35)`(bool block)` 

Return a new sample of all of the data.

#### Parameters
* `Block` if `true`, wait for data to be returned. Defaults to `false`.

#### `public bool `[`newData`](#classHaar_1ae51c1fcc10010c56263e9df311a83486)`()` 

Checks for updated data. Returns `true` if new data are available; otherwise returns `false`

#### Parameters
* `Block` if `true`, wait for data to be returned. Defaults to `false`.

#### `public String `[`getString`](#classHaar_1a3c36187bfc3d8254ff28446c315e60d7)`()` 

The most important function for the user! Returns all data as a comma-separated string: "P,RH,T(P),T[RH],".

This string is: PRESSURE,RELATIVE_HUMIDITY, TEMPERATURE_FROM_PRESSURE_SENSOR,TEMPERATURE_FROM_RH_SENSOR,

#### `public String `[`getHeader`](#classHaar_1a271e4eb4e28723df81c1a0ed1d2d0ead)`()` 

Returns a header: "Pressure Atmos [mBar], Humidity [%], Temp Pres [C], Temp RH [C],".

Generated by [Moxygen](https://sourcey.com/moxygen)