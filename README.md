# MobiusControl - Code to suit Arduino/ESP32 Module
Control *2x Ecotech Marine MOBIUS BLE Devices to enable "Feed Mode" - Code has been tested and works (at least for me :P) on Expressif ESP32-WROOM-32 and ESP32S3 modules to Control the Feed Mode for 2x Vortech MP60's inside of Home Assistant via MQTT. Will work on other ESP Based Modules however may require changes to the code.

*Optimised the code in the ESP32_MobiusBLE Library to Connect to 2 devices reliably, I modified these two files in "ESP32_MobiusBLE\src\" - MobiusDevice.cpp and MobiusDevice.h

MobiusDevice.cpp - Line 462/463
MobiusDevice.h - Line 58 - The "expectedCount" = area

***NOTE:*** All code in the Libraries directory is not mine, See the relevant Readme/Licence files in each directory for more information. I simply compiled together my code (MobiusControl.ino) with the libraries that worked with the hope that more people will have success with this project/hobby and hopefully build upon this and use this GitHub repo as a common base moving forward as when i started looking at this, files, code etc was either all over the place, based on a myriad of technologies, half-finished or non existent..  

## Instructions (What i used with code working as-is)
***STEP-1:*** Install Arduino IDE v2.3.2 (Windows\Mac\Linux): https://www.arduino.cc/en/software or https://github.com/arduino/arduino-ide/releases/tag/2.3.2

***STEP-2:*** Install USB Driver for ESP32 Boards (Windows\Mac\Linux): https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers?tab=downloads - On Windows - Download the "CP210x Universal Windows Driver" zip file, Extract and then browse the extracted directory, right-click on silabser.inf and select install and follow any prompts. Then connect your ESP32 Device to your USB Port on your computer... Should appear in your Device Manager as a COM Port...

***STEP-3:*** Run Arduino IDE and go to File > Preferences - Down the bottom where it says "Additional boards manager URLs:" add: https://espressif.github.io/arduino-esp32/package_esp32_index.json and then click on OK. You may get a pop-up saying "Downloading index: package_esp32_index.json" - Wait for this to finish (the popup will disappear on its own), if doesn't appear, close and reopen the Arduino IDE Software to refresh. 

***STEP-4:*** In Arduino IDE, Go to Tools > Board > Boards Manager - For Type - Select All and then in the "Filter your search" section type in esp32. You should see "esp32 by Expressif Systems" - Choose from the version drop-down list v2.0.17 and click on INSTALL - THIS IS IMPORTANT as this code has issues compiling with v3.0/3.0.1 or higher.

***STEP-5:*** Copy the libraries from inside the MobiusControl\Libraries directory into your Documents\Arduino\Libraries directory - (If you are on a WINDOWS PC) or wherever the equivalent is on a Mac/Linux...

***STEP-6:*** Copy the MobiusControl.ino file to Documents\Arduino directory - (If you are on a WINDOWS PC) or wherever the equivalent is on a Mac/Linux...

***STEP-7:*** Edit the following lines of MobiusControl.ino code in Arduino IDE with your values - Lines: 10 / 11 / 12 / 13 / 14 / 15 / 16 & 107


## NOTES: 
Whenever you open the Arduino IDE app it will ask to update libraries etc. DO NOT UPDATE unless you want to potentially bugfix code due to new bugs/function changes etc. You can google how to disable auto updates in Arduino IDE etc...

If modifying Arduino Library files, do so with Arduino IDE software closed and then before opening the Arduino IDE software again, clear out your C:\Windows\Temp and C:\Users\Your Username\AppData\Local\Temp directory as on Windows machines anyway, thats where all the Arduino temp files are located and we want to make sure that a rebuild of the libraries is enforced so you know 100% your changes applied and were ok.

If you have an issue compiling due to ArduinoJson.h saying it is "missing" - Try moving the #include <ArduinoJson.h> line further up the list and run a Verify/Compile. 

I quickly got frustrated and gave up so ended up editing all the files inside of ArdinoJson folder. See **ArduinoJson(Modified).zip**

Extract and then run through a find/replace on each file - Replace C:\Users\Daniel Rossi\Documents\Arduino\libraries\ with your equivalent on all files that find/replace finds C:\Users\Daniel Rossi\Documents\Arduino\libraries\ ... If you do go down the rabbit hole of modifying ArduinoJson.h files then change the code in the Sketch from #include <ArduinoJson.h> to (for example) #include "C:\\Users\\Daniel Rossi\\Documents\\Arduino\\libraries\\ArduinoJson\\ArduinoJson.h"
