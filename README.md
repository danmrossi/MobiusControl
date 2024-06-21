# MobiusControl - Code to suit Arduino/ESP32 Module

Control *2x Ecotech Marine MOBIUS BLE Devices to enable "Feed Mode" - Code has been tested and works (at least for me :P) on Expressif ESP32-WROOM-32 and ESP32S3 modules to Control the Feed Mode for 2x Vortech MP60's inside of Home Assistant via MQTT. Will work on other ESP Based Modules however may require changes to the code.


*Optimised the code in the ESP32_MobiusBLE Library to Connect to 2 devices reliably, I modified these two files in "Libraries\ESP32_MobiusBLE\src\" - MobiusDevice.cpp and MobiusDevice.h - MobiusDevice.cpp - Line 462/463 and MobiusDevice.h - Line 58 - The "expectedCount" = area. I have included the Original, Unmodifed code (ESP32_MobiusBLE(Original-Unmodified) folder.

This code should work with other "Mobius" enabled devices but you will have to play with the code to suit. What i am providing is everything i used to get it to do WHAT I WANT and to give you a decent starting point which is more than i had lol :P 

***NOTES:***


## MobiusControl-v1.ino - "Basic Sketch" 
* **Connects to 1 / 2 devices**
* **Publishes a MQTT On/Off "Feed Mode" switch**
* **Publishes a MQTT Sensor "QTY Devices"**
  * ***The number of Mobius Devices detected***
* **Publishes a MQTT Sensor caled "FM Status"**
  * ***To report wether or not turning "Feed Mode" was successful or not***
* **Includes some basic rudimentary fail safes**

## MobiusControl-v2.ino - "The Sketch i am using myself" - Continually improving upon... - 
* **Connects to 2 devices**
  * ***Only have 2 to test with***
* **Publishes a MQTT On/Off "Feed Mode" switch**
* **Publishes a MQTT Sensor "QTY Devices"**
  * ***The number of Mobius Devices detected***
* **Publishes a MQTT Sensor caled "FM Status"**
  * ***To report wether or not turning "Feed Mode" was successful or not***
* **Publishes a MQTT Sensor called "FM Timer"**
  * ***As in my Mobius app, for my situation/fish requirements etc, i have created a 18hr feed scene so that all food is eaten before the pumps turn on and blow it around. When "Feed Mode" is turned on, it counts down from 17hrs 58mins (lets me know how much time is 
       left / Keeps WiFi Alive) and when it hits zero it then turns "Feed Mode" off. I did it this way as the devices have a hissy fit if you try to turn "Feed Mode" off after the pumps already have themselves***
* **Publishes a MQTT ESP32 Restart switch**
  * ***This is so i can script in Home Assistant the ability to periodically reboot the ESP32 Module***
* **Includes ***ALOT*** of fail-safes to cover scenarios i ran into over time**     


## Instructions (What i used with code working as-is)


***STEP-1:*** Install Arduino IDE v2.3.2 (Windows\Mac\Linux): https://www.arduino.cc/en/software or https://github.com/arduino/arduino-ide/releases/tag/2.3.2

***STEP-2:*** Install USB Driver for ESP32 Boards (Windows\Mac\Linux): https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers?tab=downloads - On Windows - Download the "CP210x Universal Windows Driver" zip file, Extract and then browse the extracted directory, right-click on silabser.inf and select install and follow any prompts. Then connect your ESP32 Device to your USB Port on your computer... Should appear in your Device Manager as a COM Port...

***STEP-3:*** Run Arduino IDE and go to File > Preferences - Down the bottom where it says "Additional boards manager URLs:" add: https://espressif.github.io/arduino-esp32/package_esp32_index.json and then click on OK. You may get a pop-up saying "Downloading index: package_esp32_index.json" - Wait for this to finish (the popup will disappear on its own), if doesn't appear, close and reopen the Arduino IDE Software to refresh. 

***STEP-4:*** In Arduino IDE, Go to Tools > Board > Boards Manager - For Type - Select All and then in the "Filter your search" section type in esp32. You should see "esp32 by Expressif Systems" - Choose from the version drop-down list v2.0.17 and click on INSTALL - THIS IS IMPORTANT as this code has issues compiling with v3.0/3.0.1 or higher.

***STEP-5:*** Extract Libraries.zip contents into your Documents\Arduino\Libraries directory - (If you are on a WINDOWS PC) or wherever the equivalent is on a Mac/Linux... **NOTE:** Replace the ArduinoJson folder in libraries folder contents with the one from ArduinoJson(Original-Unmodified) if using the sketch as is. If you have an issue compiling due to ArduinoJson.h saying it is "missing" - Try moving the #include <ArduinoJson.h> line further up the list and run a Verify/Compile each time and move up again until it works. 

I quickly got frustrated and gave up so ended up editing all the files inside of ArduinoJson folder (The current ArduinoJson folder in the Libraries directory. 

To modify the files to suit your location - find/replace on each file (on my version of ArduinoJson in the Libraries directory) - Replace C:\Users\Daniel Rossi\Documents\Arduino\libraries\ with your equivalent on all files that find/replace finds C:\Users\Daniel Rossi\Documents\Arduino\libraries\ ... If you do go down the rabbit hole of modifying all the ArduinoJson files then change the code in the Sketch from #include <ArduinoJson.h> to (for example) #include "C:\\Users\\Daniel Rossi\\Documents\\Arduino\\libraries\\ArduinoJson\\ArduinoJson.h"

***STEP-6:*** Copy the MobiusControl.ino file to Documents\Arduino directory - (If you are on a WINDOWS PC) or wherever the equivalent is on a Mac/Linux...

***STEP-7:*** For MobiusControl-v1.ino - Edit the following lines in Arduino IDE with your values - Lines: 10 / 11 / 12 / 13 / 14 / 15 / 16 & 107 - For MobiusControl-v2.ino - Edit the following lines in Arduino IDE with your values - Lines: 10 / 11 / 12 / 13 / 14 / 15 / 16 & 124


***TROUBLESHOOTING:*** 


If you are having random issues with Bluetooth connectivity, Set the 2.4ghz WiFi Channel on your Router to Channel 1. This prevents any WiFi Channel overlap with the Mobius Device / ESP32 Bluetooth module signals. 

To make sure that WiFi connectivity is also constant with ESP32 devices, i find that if you set your DHCP Range in your Router to be smaller for example - Set it to hand out IP Addresses to all your devices between 192.168.1.50 - 192.168.1.100 (or whatever your 192.168. range is) and then also in your Router set your ESP32 device with a Static IP of an Address outside of this DHCP Range for example 192.168.1.101 - It seems to make ESP32 device connections more stable and consistant to your Router and Home Assistant/MQTT Broker.

Whenever you open the Arduino IDE app it will ask to update libraries etc. **DO NOT UPDATE** unless you want to potentially bugfix code due to new bugs/function changes etc. You can google how to disable auto updates in Arduino IDE etc...

If modifying Arduino Library files, do so with Arduino IDE software closed and then before opening the Arduino IDE software again, clear out your C:\Windows\Temp and C:\Users\Your Username\AppData\Local\Temp directory as on Windows machines anyway, thats where all the Arduino temp files are located and we want to make sure that a rebuild of the libraries is enforced so you know 100% your changes applied and were ok.





***All code in the Libraries directory is not mine, See the relevant Readme/Licence files located within each directory for more information.*** 

I simply compiled together my code (MobiusControl-v1.ino & Mobiuscontrol-v2.ino) with the libraries that worked with the hope that more people will have success with this project/hobby and hopefully build upon this and use this GitHub repo as a common base moving forward as when i started looking at this, files, code etc was either all over the place, based on a myriad of technologies, half-finished or parts non existent..
