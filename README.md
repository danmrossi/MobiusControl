# MobiusControl Arduino/ESP32 Module Code
Code has been tested and works on Expressif ESP32-WROOM-32 and ESP32S3 modules. Will work on others but may require changes to the code...


## Software & Versions i utilised to create & compile the Code:
Software for writing the Code & Uploading to ESP32 Modules: Arduino IDE v2.3.2 from: https://www.arduino.cc/en/software or https://github.com/arduino/arduino-ide/releases/tag/2.3.2

## Software & Versions i utilised to install for Arduino IDE ESP32 device support:
Run Arduino IDE and go to File > Preferences - Down the bottom where it says "Additional boards manager URLs:" add: https://espressif.github.io/arduino-esp32/package_esp32_index.json and then click on OK. You may get a pop-up saying "Downloading index: package_esp32_index.json" Wait for this to finish (the popup will disappear on its own), if doesn't appear, close and reopen the Arduino IDE Software. Either way you then should be safe to then go to Tools > Board > Boards Manager - For Type - Select All and then in the "Filter your search" section type in esp32. You should see "esp32 by Expressif Systems" - Choose from the version drop-down list v2.0.17 and click on INSTALL - THIS IS IMPORTANT as the code has issues compiling with v3.0/3.0.1 or higher.

## ESP32 Serial Driver (Drivers for the ESP32/ESP32S3 Board for Windows/Mac/Linux: 
https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers?tab=downloads - On Windows - Download the "CP210x Universal Windows Driver" zip file, Extract and then browse the extracted directory, right-click on silabser.inf and select install and follow any prompts. Then connect your ESP32 Device to your USB Port on your computer...

## NOTE: 
Whenever you open the Arduino IDE app it will ask to update libraries etc. DO NOT UPDATE unless you want to bugfix code. You can google how to disable auto updates in Arduino IDE etc...
When following the instructions below, perform with the Arduino IDE software not running...


## Instructions
Copy the libraries from inside the MobiusControl\Libraries directory into your Documents\Arduino\Libraries directory - (If you are on a WINDOWS PC) or wherever the equivalent is on a Mac/Linux...
Copy the MobiusControl.ino file to Documents\Arduino directory - (If you are on a WINDOWS PC) or wherever the equivalent is on a Mac/Linux...
