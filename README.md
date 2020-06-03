# NorthernWidget-libraries

***Arduino libraries for all Northern Widget data loggers and sensors.***

These libraries are all copies from their original sources. Most are from Northern Widget. Some are not.

## Installation instructions

This package bundles together many libraries for ease of download. However, this means that you cannot use the [Arduino Library Manager](https://www.arduino.cc/en/guide/libraries#toc3) built into the [Arduino IDE](https://www.arduino.cc/en/main/software). Therefore, we have to use the older approach of copying the folders containing the library information to the folder containing your Arduino libraries.

### Arduino libraries folder location

Your Arduino libraries are held in a folder called **libraries**. Who would have guessed? Here is where to find it:

#### Linux

**Installation via direct download**

*Highly recommended: download most recent version for your OS (most likely "Linux 64 bits") from https://www.arduino.cc/en/main/software*

*Libraries location:* `$HOME/Arduino/libraries`

**Installation from apt**
You can use:
```
apt install arduino
```
This can help to ensure that you have important core libraries, but this version of Arduino is often significantly out of date.

*Libraries location:* `$HOME/.arduino/libraries`

#### Windows

*Libraries location:* `'My Documents\Arduino\libraries'`

#### Mac

To view the libraries on Mac, you have to open the contents of your **ArduinoNNN.app** package, where NNN stands for the version number. (CTRL+click --> show package contents.)

*Libraries location within Arduino app package:* `Contents/Resources/Java/libraries`

### Download


### Copy/paste contents


## External resources

* https://learn.sparkfun.com/tutorials/installing-an-arduino-library
* https://www.arduino.cc/en/guide/libraries#toc5
