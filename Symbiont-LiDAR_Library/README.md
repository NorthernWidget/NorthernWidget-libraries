# Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[`SymbiontLiDAR`](#classSymbiontLiDAR) | Arduino library for the Symbiont board, which manages a LiDAR Lite unit (roll/pitch, firmware lock/reset, power supply).

# class `SymbiontLiDAR` 

Arduino library for the Symbiont board, which manages a LiDAR Lite unit (roll/pitch, firmware lock/reset, power supply).

Interface with the [SymbiontLiDAR](#classSymbiontLiDAR) module.

Library to communicate with the [SymbiontLiDAR](#classSymbiontLiDAR) module, which connects to a LiDAR Lite rangefinder.The Symbiont is equipped with capacitors to handle the large burst power draw from the LiDAR Lite, a MEMS accelerometer to note its orientation, a magnet to note a known orientation (often, but not necessarily, horizontal) and the ability to absorb occasional firmware issues that lead to system hangs. The leveling helps the user to calculate, for example, a water level when the sensor is palced on a cliff a tree next to the river but does not have water below it. The level loses absolute accuracy when near plumb, so a Hall-effect sensor connected to the magnet allows the user to set a zero value, thereby correcting for this. Managing failures of the LiDAR Lite within the Symbiont is essential, and the Symbiont therefore acts as a buffer to protect the data logger from raw sensor failures.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  `[`SymbiontLiDAR`](#classSymbiontLiDAR_1af16b0ce31efd8892bfc42ed9f1f13317)`()` | Instantiate [SymbiontLiDAR](#classSymbiontLiDAR) object.
`public void `[`begin`](#classSymbiontLiDAR_1ad5d076629b619e6b17cb2b68bf459366)`(uint8_t Address_,uint8_t sensitivity)` | Begin communications with the Symbiont using a prescribed address.
`public bool `[`updateMeasurements`](#classSymbiontLiDAR_1a5bdfb70ff9bd1f02804667c698c0cf87)`()` | Measure range [cm], roll [deg] and pitch [deg]. Returns "0" if sensors read error values; else returns "1".
`public float `[`getRange`](#classSymbiontLiDAR_1abdd716a0f55526d57f31b853f41280cb)`()` | Return range [cm].
`public float `[`getRoll`](#classSymbiontLiDAR_1a3ed32c762addeec50be410aca132e16e)`()` | Return roll [deg].
`public float `[`getPitch`](#classSymbiontLiDAR_1ad06bac488cb930345d023903e74844a3)`()` | Return pitch [deg].
`public String `[`getHeader`](#classSymbiontLiDAR_1a3637c11f25bc19257458a1947f67cdb6)`()` | Return a comma-separated header: "Range [cm],Pitch [deg],Roll [deg],".
`public String `[`getString`](#classSymbiontLiDAR_1a0f21c278f00301fd35955fa0311cd7f8)`(bool takeNewReadings)` | Return comma-separated data values.
`public String `[`GetHeader`](#classSymbiontLiDAR_1a4c83d0aa98cc37aa76cff14a7ccc9e61)`()` | CapitalCase version of [getHeader()](#classSymbiontLiDAR_1a3637c11f25bc19257458a1947f67cdb6) for backwards compatibility.
`public String `[`GetString`](#classSymbiontLiDAR_1a454c7b9749d3735178af599dd8cb3b79)`(bool takeNewReadings)` | CapitalCase version of [getString()](#classSymbiontLiDAR_1a0f21c278f00301fd35955fa0311cd7f8) for backwards compatibility.

## Members

#### `public  `[`SymbiontLiDAR`](#classSymbiontLiDAR_1af16b0ce31efd8892bfc42ed9f1f13317)`()` 

Instantiate [SymbiontLiDAR](#classSymbiontLiDAR) object.

#### `public void `[`begin`](#classSymbiontLiDAR_1ad5d076629b619e6b17cb2b68bf459366)`(uint8_t Address_,uint8_t sensitivity)` 

Begin communications with the Symbiont using a prescribed address.

#### Parameters
* `Addresss_` default 0x40 DOES NOT YET USE A VARIABLE ADDRESS! DOES NOT MATTER WHAT YOU WRITE HERE. THIS DEFAULT ADDRESS CURRENTLY CLASHES WITH HAAR'S DEFAULT! 

* `sensitivity.` Options are: sensitivityBalanced (0) sensitivityHigh (1) sensitivityLow (2) sensitivityMaxRange (3) Default value: sensitivityBalanced (0) THIS IS ALSO NOT YET IMPLEMENTED IN FIRMWARE! JUST STAYS AT DEFAULT (SENSITIVITY_BALANCED = 0) CONSIDER A SET_SENSITIVITY FUNCTION IN THE FUTURE TO ALLOW THE SENSOR TO AUTOMATICALLY ADJUST THE SENSITIVITY TO RECEIVE THE BEST POSSIBLE RETURN

#### `public bool `[`updateMeasurements`](#classSymbiontLiDAR_1a5bdfb70ff9bd1f02804667c698c0cf87)`()` 

Measure range [cm], roll [deg] and pitch [deg]. Returns "0" if sensors read error values; else returns "1".

#### `public float `[`getRange`](#classSymbiontLiDAR_1abdd716a0f55526d57f31b853f41280cb)`()` 

Return range [cm].

#### `public float `[`getRoll`](#classSymbiontLiDAR_1a3ed32c762addeec50be410aca132e16e)`()` 

Return roll [deg].

#### `public float `[`getPitch`](#classSymbiontLiDAR_1ad06bac488cb930345d023903e74844a3)`()` 

Return pitch [deg].

#### `public String `[`getHeader`](#classSymbiontLiDAR_1a3637c11f25bc19257458a1947f67cdb6)`()` 

Return a comma-separated header: "Range [cm],Pitch [deg],Roll [deg],".

#### `public String `[`getString`](#classSymbiontLiDAR_1a0f21c278f00301fd35955fa0311cd7f8)`(bool takeNewReadings)` 

Return comma-separated data values.

This is the most likely function (alongside getHeader) for an end user to use. Values are returned as: String(Range) + "," + String(Pitch) + "," + String(Roll) + "," Error values are "-9999" (sensor error) and "-9998" (no measurement yet taken") 
#### Parameters
* `takeNewReadings` if `true` run `updateMeasurements` before returning values. Otherwise, just return values.

#### `public String `[`GetHeader`](#classSymbiontLiDAR_1a4c83d0aa98cc37aa76cff14a7ccc9e61)`()` 

CapitalCase version of [getHeader()](#classSymbiontLiDAR_1a3637c11f25bc19257458a1947f67cdb6) for backwards compatibility.

#### `public String `[`GetString`](#classSymbiontLiDAR_1a454c7b9749d3735178af599dd8cb3b79)`(bool takeNewReadings)` 

CapitalCase version of [getString()](#classSymbiontLiDAR_1a0f21c278f00301fd35955fa0311cd7f8) for backwards compatibility.

#### Parameters
* `takeNewReadings` if `true` run `updateMeasurements` before returning values. Otherwise, just return values.

Generated by [Moxygen](https://sourcey.com/moxygen)