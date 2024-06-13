# MobiusControl (Arduino/ESP32 Module)
Control *2x Ecotech Marine MOBIUS BLE Devices to enable "Feed Mode" - Code has been tested and works on Expressif ESP32-WROOM-32 and ESP32S3 modules. Will work on others will most likely require changes to the code...

*Optimised the code to Connect to 2 devices reliably.............


Then connect your ESP32 Device to your USB Port on your computer...

## NOTES: 
Whenever you open the Arduino IDE app it will ask to update libraries etc. DO NOT UPDATE unless you want to bugfix code. You can google how to disable auto updates in Arduino IDE etc...

When following the instructions below, perform with the Arduino IDE software not running...

If you have an issue compiling due to ArduinoJson.h saying it is "missing" - Try moving the #include <ArduinoJson.h> line further up the list and run a Verify/Compile. I quickly got frustrated and gave up so ended up editing all the files inside of ArdinoJson folder. See ArduinoJson(Modified).zip - Extract and then run through a find/replace on each file - Replace C:\Users\Daniel Rossi\Documents\Arduino\libraries\ with your equivalent on all files that find/replace finds C:\Users\Daniel Rossi\Documents\Arduino\libraries\ ... If you do go down the rabbit hole of modifying ArduinoJson.h files then change the code in the Sketch from #include <ArduinoJson.h> to (for example) #include "C:\\Users\\Daniel Rossi\\Documents\\Arduino\\libraries\\ArduinoJson\\ArduinoJson.h" ...     

## Instructions (What i used with code working as-is)
STEP-1: Install Arduino IDE v2.3.2 (Windows\Mac\Linux): https://www.arduino.cc/en/software or https://github.com/arduino/arduino-ide/releases/tag/2.3.2

STEP-2: Install USB Driver for ESP32 Boards (Windows\Mac\Linux): https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers?tab=downloads - On Windows - Download the "CP210x Universal Windows Driver" zip file, Extract and then browse the extracted directory, right-click on silabser.inf and select install and follow any prompts. 

STEP-3: Run Arduino IDE and go to File > Preferences - Down the bottom where it says "Additional boards manager URLs:" add: https://espressif.github.io/arduino-esp32/package_esp32_index.json and then click on OK. You may get a pop-up saying "Downloading index: package_esp32_index.json" - Wait for this to finish (the popup will disappear on its own), if doesn't appear, close and reopen the Arduino IDE Software to refresh. 

STEP-4: In Arduino IDE, Go to Tools > Board > Boards Manager - For Type - Select All and then in the "Filter your search" section type in esp32. You should see "esp32 by Expressif Systems" - Choose from the version drop-down list v2.0.17 and click on INSTALL - THIS IS IMPORTANT as this code has issues compiling with v3.0/3.0.1 or higher.

STEP-5: Copy the libraries from inside the MobiusControl\Libraries directory into your Documents\Arduino\Libraries directory - (If you are on a WINDOWS PC) or wherever the equivalent is on a Mac/Linux...

STEP-6: Copy the MobiusControl.ino file to Documents\Arduino directory - (If you are on a WINDOWS PC) or wherever the equivalent is on a Mac/Linux...

STEP-7: Change the following areas of code with your values:


