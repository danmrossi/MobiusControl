#include <ESP32_MobiusBLE.h>
#include "ArduinoSerialDeviceEventListener.h"
#include "EspMQTTClient.h"
#include <WiFi.h>
#include <esp_bt.h>
#include <string>
#include <ArduinoJson.h>

// MQTT client object with parameters for WiFi and MQTT broker
EspMQTTClient client(
  "YourWiFiSSIDHere",
  "YourWiFiPasswordHere",
  "YourHomeAssistantIPAddressHere",
  "YourMQTTBrokerUsernameHere",
  "YourMQTTBrokerPasswordHere",
  "Mobius",
  1883
);

// JSON discovery messages for Home Assistant
const char* jsonSwitchDiscovery =  R"json({
    "name":"Feed Mode",
    "command_topic":"homeassistant/switch/mb/set",
    "state_topic":"homeassistant/switch/mb/state",
    "unique_id":"fmsw",
    "device":{
      "identifiers":[
        "mb01"
      ],
      "name":"Esp32-MC"
    }
})json";

const char* jsonSensorDiscovery =  R"json({
    "name":"QTY Devices",
    "state_topic":"homeassistant/sensor/mb/state",
    "value_template":"{{ value_json.qtydevices}}",
    "unique_id":"qtydev",
    "device":{
      "identifiers":[
        "mb01"
      ]
    }
})json";

const char* jsonStatusDiscovery =  R"json({
    "name":"FM Status",
    "state_topic":"homeassistant/sensor/mb/status",
    "unique_id":"fmst",
    "device":{
      "identifiers":[
        "mb01"
      ]
    }
})json";

const char* jsonTimerDiscovery =  R"json({
    "name":"FM Timer",
    "state_topic":"homeassistant/sensor/mb/timer",
    "unique_id":"fmtimer",
    "device":{
      "identifiers":[
        "mb01"
      ]
    }
})json";

// New JSON discovery message for the restart switch
const char* jsonRestartDiscovery =  R"json({
    "name":"MQTT Restart",
    "command_topic":"homeassistant/switch/restart/set",
    "state_topic":"homeassistant/switch/restart/state",
    "unique_id":"restartsw",
    "device":{
      "identifiers":[
        "mb01"
      ],
      "name":"Esp32-MC"
    }
})json";

// State variables
bool prevState = false;
bool currState = false;
bool configPublish = false;
bool firstRun = true;
unsigned long feedModeStartMillis = 0;  // Timer for Feed Mode
const unsigned long feedModeDuration = 64680000;  // 17 hours and 58 minutes in milliseconds

enum State {
  IDLE,
  SCANNING,
  PUBLISH_CONFIG,
  PUBLISH_STATE,
  HANDLE_DEVICES
};

State currentState = IDLE;
unsigned long lastMillis = 0;
const unsigned long scanInterval = 10000; // 10 seconds scan interval
const unsigned long stateUpdateInterval = 3000; // 3 seconds delay for state updates

MobiusDevice deviceBuffer[30];
int deviceCount = 0;
int currentDeviceIndex = 0;
int connectTries = 0;
unsigned long lastReconnectAttempt = 0;
const unsigned long reconnectInterval = 10000; // 10 seconds reconnect interval

// Callback function when MQTT connection is established
void onConnectionEstablished() {
  Serial.println("CONNECTED TO MQTT BROKER");

  // Subscribe to the feed mode switch topic
  client.subscribe("homeassistant/switch/mb/set", [](const String& feedMode) {
    if (feedMode.length() > 0) {
      if (feedMode == "ON") {
        currState = true;
        feedModeStartMillis = millis();  // Record the start time
      } else {
        currState = false;
        feedModeStartMillis = 0;  // Reset the start time
      }
      configPublish = false;
      if (!client.publish("homeassistant/switch/mb/state", feedMode, true)) {
        Serial.printf("ERROR: DID NOT PUBLISH FEED SWITCH\n");
      }
      if (feedMode == "OFF") {
        if (!client.publish("homeassistant/sensor/mb/timer", "Idle", true)) {
          Serial.printf("ERROR: DID NOT PUBLISH TIMER STATUS\n");
        }
      }
    }
  });

  // Subscribe to the restart switch topic
  client.subscribe("homeassistant/switch/restart/set", [](const String& restartCommand) {
    Serial.printf("Restart command received: %s\n", restartCommand.c_str());
    if (restartCommand == "ON") {
      Serial.println("MQTT restart command received, restarting ESP32...");
      delay(1000);  // Small delay before restarting to ensure the message is processed
      ESP.restart();  // Restart the ESP32
    }
  });
}

// Ensure WiFi is connected
void ensureWiFiConnected() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin("HomeWireless-2.4", "Pc265jess4@");
    unsigned long startAttemptTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
      delay(500);
      Serial.print(".");
    }
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("WiFi reconnected");
    } else {
      Serial.println("ERROR: WiFi reconnection failed after 10 seconds.");
    }
  }
}

// Ensure MQTT is connected
void ensureMQTTConnected() {
  if (!client.isConnected()) {
    unsigned long now = millis();
    if (now - lastReconnectAttempt > reconnectInterval) {
      lastReconnectAttempt = now;
      Serial.println("Reconnecting to MQTT...");
      client.loop();
      if (client.isConnected()) {
        lastReconnectAttempt = 0; // Reset the reconnect attempt counter
        Serial.println("Reconnected to MQTT.");
      } else {
        Serial.println("ERROR: Reconnection attempt to MQTT failed.");
      }
    }
  }
}

// Ensure Bluetooth is initialized
void ensureBluetoothInitialized() {
  static int bluetoothInitRetries = 0;
  const int maxRetries = 3;
  if (deviceCount == 0 && bluetoothInitRetries < maxRetries) {
    bluetoothInitRetries++;
    Serial.println("Reinitializing Bluetooth...");
    MobiusDevice::init(new ArduinoSerialDeviceEventListener());
    delay(2000);  // Added delay after Bluetooth initialization
  } else {
    bluetoothInitRetries = 0; // Reset retry counter if devices are found
  }
}

// Scan for devices with a retry mechanism
void scanForDevices() {
  delay(1000);  // Delay before scanning
  int retries = 3;
  deviceCount = 0;
  for (int i = 0; i < retries; i++) {
    int tempDeviceCount = MobiusDevice::scanForMobiusDevices(scanInterval / 1000, deviceBuffer, 30);
    deviceCount = max(deviceCount, tempDeviceCount);
    if (deviceCount > 0) break;
    delay(2000);
  }
  Serial.printf("INFO: MOBIUS BLE DEVICES FOUND: %i\n", deviceCount);
  ensureBluetoothInitialized(); // Reinitialize Bluetooth if no devices found
  if (deviceCount == 0) {
    Serial.println("ERROR: No Mobius BLE devices found after scanning.");
  }
  delay(1000);  // Delay after scanning
}

// Publish configuration messages to MQTT
void publishConfigs() {
  delay(1000);  // Delay before publishing configs
  if (!client.publish("homeassistant/switch/mb/config", jsonSwitchDiscovery, true)) {
    Serial.printf("ERROR: DID NOT PUBLISH SWITCH CONFIG\n");
  }
  if (!client.publish("homeassistant/sensor/mb/config", jsonSensorDiscovery, true)) {
    Serial.printf("ERROR: DID NOT PUBLISH SENSOR CONFIG\n");
  }
  if (!client.publish("homeassistant/sensor/mb/status/config", jsonStatusDiscovery, true)) {
    Serial.printf("ERROR: DID NOT PUBLISH STATUS CONFIG\n");
  }
  if (!client.publish("homeassistant/sensor/mb/timer/config", jsonTimerDiscovery, true)) {
    Serial.printf("ERROR: DID NOT PUBLISH TIMER CONFIG\n");
  }
  if (!client.publish("homeassistant/switch/restart/config", jsonRestartDiscovery, true)) {
    Serial.printf("ERROR: DID NOT PUBLISH RESTART SWITCH CONFIG\n");
  }
  configPublish = true;
  delay(1000);  // Delay after publishing configs
}

// Update device state
bool updateDeviceState(MobiusDevice& device) {
  const int maxRetries = 3;
  for (int i = 0; i < maxRetries; i++) {
    Serial.println("Starting connection to the device");
    delay(1000);  // Delay before each connection attempt
    if (device.connect()) {
      Serial.println("Successfully connected to the device");
      delay(1000);  // Delay after connecting
      uint16_t sceneId = device.getCurrentScene();
      Serial.printf("INFO: SCENE ID FOR DEVICE: %u\n", sceneId);

      if (currState) {
        device.setFeedScene();
      } else {
        device.runSchedule();
      }

      delay(1000);  // Delay before disconnecting
      device.disconnect();
      Serial.println("Successfully disconnected from the device");
      delay(1000);  // Delay after disconnecting
      return true;
    } else {
      Serial.printf("ERROR: FAILED TO CONNECT TO DEVICE (Try %i)\n", i + 1);
      delay(3000); // Increased delay before retrying (3 seconds)
    }
  }
  Serial.println("ERROR: EXCEEDED MAX RETRIES FOR DEVICE CONNECTION");
  return false;
}

// Publish MQTT message with retry mechanism
bool publishWithRetry(const char* topic, const char* payload, bool retain = false, int retries = 3) {
  for (int i = 0; i < retries; i++) {
    if (client.publish(topic, payload, retain)) {
      return true;
    }
    delay(1000);  // Wait for a second before retrying
  }
  Serial.printf("ERROR: FAILED TO PUBLISH MESSAGE TO TOPIC: %s AFTER %d RETRIES\n", topic, retries);
  return false;
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {};

  WiFi.setHostname("esp32-mc");

  prevState = !currState;
  firstRun = true;

  client.setMaxPacketSize(2048);  // Adjust size as needed
  client.setKeepAlive(60);  // 60 seconds keep-alive interval

  esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT); // Release classic BT memory to reduce RAM usage

  MobiusDevice::init(new ArduinoSerialDeviceEventListener());
  Serial.println("SETUP RUN");
  delay(2000);  // Delay at the end of setup
}

void loop() {
  ensureWiFiConnected();
  ensureMQTTConnected();
  client.loop();

  // Update timer status if feed mode is active
  if (currState && feedModeStartMillis > 0) {
    unsigned long elapsedMillis = (millis() - feedModeStartMillis) % feedModeDuration;
    if (elapsedMillis >= feedModeDuration) {
      currState = false;
      feedModeStartMillis = 0;  // Reset the timer
      if (!client.publish("homeassistant/switch/mb/state", "OFF", true)) {
        Serial.printf("ERROR: DID NOT PUBLISH FEED SWITCH OFF\n");
      }
      if (!client.publish("homeassistant/sensor/mb/timer", "Idle", true)) {
        Serial.printf("ERROR: DID NOT PUBLISH TIMER STATUS\n");
      }
    } else {
      unsigned long remainingMillis = feedModeDuration - elapsedMillis;
      unsigned long hours = remainingMillis / 3600000;
      unsigned long minutes = (remainingMillis % 3600000) / 60000;
      unsigned long seconds = (remainingMillis % 60000) / 1000;
      char timerStatus[50];
      snprintf(timerStatus, sizeof(timerStatus), "%lu:%02lu:%02lu", hours, minutes, seconds);
      client.publish("homeassistant/sensor/mb/timer", timerStatus, true);
    }
  }

  switch (currentState) {
    case IDLE:
      if (millis() - lastMillis > scanInterval) {
        lastMillis = millis();
        currentState = SCANNING;
      }
      break;

    case SCANNING:
      scanForDevices();
      currentState = PUBLISH_CONFIG;
      break;

    case PUBLISH_CONFIG:
      if (!configPublish) {
        publishConfigs();
        delay(stateUpdateInterval);
        currentState = PUBLISH_STATE;
      } else {
        currentState = PUBLISH_STATE;
      }
      break;

    case PUBLISH_STATE:
      if (prevState != currState) {
        if (!firstRun && deviceCount > 0) {
          currentDeviceIndex = 0;
          connectTries = 0;
          currentState = HANDLE_DEVICES;
        }

        char cstr[20];
        StaticJsonDocument<100> jsonQtyDev;
        jsonQtyDev["qtydevices"] = deviceCount;
        serializeJson(jsonQtyDev, cstr, sizeof(cstr));
        if (!publishWithRetry("homeassistant/sensor/mb/state", cstr, true)) {
          Serial.printf("ERROR: DID NOT PUBLISH DEVICE QUANTITY\n");
        }

        prevState = currState;
        firstRun = false;

        // Only publish timer status if feed mode state changed
        if (currState) {
          unsigned long remainingMillis = feedModeDuration;
          unsigned long hours = remainingMillis / 3600000;
          unsigned long minutes = (remainingMillis % 3600000) / 60000;
          unsigned long seconds = (remainingMillis % 60000) / 1000;
          char timerStatus[50];
          snprintf(timerStatus, sizeof(timerStatus), "%lu:%02lu:%02lu", hours, minutes, seconds);
          if (!client.publish("homeassistant/sensor/mb/timer", timerStatus, true)) {
            Serial.printf("ERROR: DID NOT PUBLISH TIMER STATUS\n");
          }
        } else {
          if (!client.publish("homeassistant/sensor/mb/timer", "Idle")) {
            Serial.printf("ERROR: DID NOT PUBLISH TIMER STATUS\n");
          }
        }
      }
      break;

    case HANDLE_DEVICES:
      if (currentDeviceIndex < deviceCount) {
        MobiusDevice& device = deviceBuffer[currentDeviceIndex];
        bool success = updateDeviceState(device);
        currentDeviceIndex++;
        if (currentDeviceIndex >= deviceCount) {
          const char* statusStr = success ? "Success" : "Failed";
          if (!publishWithRetry("homeassistant/sensor/mb/status", statusStr, true)) {
            Serial.printf("ERROR: DID NOT PUBLISH STATUS\n");
          }
          currentState = IDLE;
        }
      } else {
        currentState = IDLE;
      }
      break;
  }
}