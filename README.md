# NorthernWidget-libraries

***Arduino libraries for all Northern Widget data loggers and sensors.***

These libraries are all copies from their original sources. Most are from Northern Widget. Some are not.

## Installation instructions

***Note: the Arduino IDE needs to be installed on your machine before installing these libraries. If you do not yet have Arduino installed, you can download it [here](https://www.arduino.cc/en/main/software).***

This package bundles together many libraries for easy downloading. However, this means that you cannot use the [Arduino Library Manager](https://www.arduino.cc/en/guide/libraries#toc3) built into the [Arduino IDE](https://www.arduino.cc/en/main/software). Therefore, we have to use the older approach of copying the folders containing the library information to the folder containing your Arduino libraries.

### Arduino libraries folder location

Your Arduino libraries are held in a folder called, conveniently, **libraries**. Here is where to find it:

### Linux

**Installation via direct download**

*Highly recommended: download most recent version for your OS (most likely "Linux 64 bit") from https://www.arduino.cc/en/main/software*

*Libraries location:* `$HOME/Arduino/libraries`

**Installation via apt**
You can use:
```
apt install arduino
```
This can help to ensure that you have important core libraries, but this version of Arduino is often significantly out of date. Be sure to double-check the Arduino version for potential compatability issues if installed this way. If problems occur, defer back to installation via direct download.

*Libraries location:* `$HOME/.arduino/libraries`

### Windows

*Libraries location:* `'My Documents\Arduino\libraries'`

### Mac

To view the libraries on Mac, you have to open the contents of your **ArduinoNNN.app** package, where NNN stands for the version number. (CTRL+click --> show package contents.)

*Libraries location within Arduino app package:* `Contents/Resources/Java/libraries`

### Download and extract contents

#### Option A: ZIP

***Better for users unfamiliar with git***

1. Click on the green "Clone or download" button on this page.
2. Click "Download ZIP"
3. Open the ZIP file. It should contain one folder titled "NorthernWidget-libraries-master"
4. Open "NorthernWidget-libraries-master" to view all of its subfolders. Each is a library.
5. Extract the subfolders within the "NorthernWidget-libraries-master" folder you would like to use to your Arduino "libraries" folder. You could also just extract all of them!
  * If this is not directly possible (perhaps for Mac), extract the contents to a temporary folder and then cut/paste them to your Arduino "libraries" folder.
  * At the risk of redundancy, for the sake of clarity the "NorthernWidget-libraries-master" folder is not what should be directly in your Arduino "libraries" folder, rather the subfolders of "NorthernWidget-libraries-master" (e.g. "Margay_Library") are what should be in your Arduino "libraries" folder

At this point, you should have all the libraries required to compile any Northern Widget code.

If you are running the Arduino IDE, you may need to restart it in order to for the changes to take effect.

Repeat these steps if you would like to obtain updated library versions.

#### Option B: git

***Easier for updating to the latest library versions***

1. Click on the green "Clone or download" button on this page.
2. Obtain the link and clone it to your Arduino "libraries" folder (e.g., using `git clone` from the command line)

At this point, you should have all the libraries required to compile any Northern Widget code.

If you are running the Arduino IDE, you may need to restart it in order to for the changes to take effect.

If you would like to obtain updated library versions, simply run `git pull` in the Arduino libraries folder.

### Reporting and fixing issues

If you have problems, please open an issue [here](https://github.com/NorthernWidget-Skunkworks/NorthernWidget-libraries/issues) if it relates to the library compilation (or you do not know), at the [firmware aggregator](https://github.com/NorthernWidget-Skunkworks/Firmware-Aggregator/issues) if it relates to our library bundling, or within the appropriate library if you can pinpoint its location.

Pull requests are very much appreciated! Please consider contributing to our community scientific instrumentation project.

## External resources

* https://learn.sparkfun.com/tutorials/installing-an-arduino-library
* https://www.arduino.cc/en/guide/libraries#toc5
