#include <ESP32_MobiusBLE.h>
#include "ArduinoSerialDeviceEventListener.h"
#include "EspMQTTClient.h"
#include <WiFi.h>
#include <string>
#include <ArduinoJson.h>

// MQTT client object with parameters for WiFi and MQTT broker
EspMQTTClient client(
  "EnterYourWifiSSIDHere",
  "EnterYourWiFiPasswordHere",
  "homeassistant.local",
  "MQTTBrokerUsername",
  "MQTTBrokerPassword",
  "Mobius",
  1883
);

// JSON discovery messages for Home Assistant
char* jsonSwitchDiscovery =  R"json({
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

char* jsonSensorDiscovery =  R"json({
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

char* jsonStatusDiscovery =  R"json({
    "name":"FM Status",
    "state_topic":"homeassistant/sensor/mb/status",
    "unique_id":"fmst",
    "device":{
      "identifiers":[
        "mb01"
      ]
    }
})json";

// State variables
bool prevState = false;
bool currState = false;
bool configPublish = false;
bool firstRun = true;

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
void onConnectionEstablished()
{
  Serial.println("CONNECTED TO MQTT BROKER");
  client.subscribe("homeassistant/switch/mb/set", [](const String& feedMode) {
    if (feedMode.length() > 0) {
      if (feedMode == "ON") {
        Serial.printf("INFO: IS IT ON: %s\n", feedMode.c_str());
        currState = true;
      } else {
        Serial.printf("INFO: IS IT OFF: %s\n", feedMode.c_str());
        currState = false;
      }
      configPublish = false;
      Serial.printf("INFO: UPDATE DEVICE SCENE FROM MQTT TRIGGER: %s\n", feedMode.c_str());
      if (!client.publish("homeassistant/switch/mb/state", feedMode)) {
        Serial.printf("ERROR: DID NOT PUBLISH FEED SWITCH");
      }
    }
  });
}

// Ensure WiFi is connected
void ensureWiFiConnected() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin("EnterYourWifiSSIDHere", "EnterYourWiFiPasswordHere");
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("WiFi reconnected");
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
      } else {
        Serial.println("Reconnection attempt failed.");
      }
    }
  }
}

// Scan for devices with a retry mechanism
void scanForDevices() {
  int retries = 3;
  deviceCount = 0;
  for (int i = 0; i < retries; i++) {
    int tempDeviceCount = MobiusDevice::scanForMobiusDevices(scanInterval / 1000, deviceBuffer, 30);
    deviceCount = max(deviceCount, tempDeviceCount);
    if (deviceCount > 0) break;
    delay(1000);
  }
  Serial.printf("INFO: MOBIUS BLE DEVICES FOUND: %i\n", deviceCount);
}

// Publish configuration messages to MQTT
void publishConfigs() {
  if (!client.publish("homeassistant/switch/mb/config", jsonSwitchDiscovery)) {
    Serial.printf("ERROR: DID NOT PUBLISH SWITCH CONFIG");
  }
  if (!client.publish("homeassistant/sensor/mb/config", jsonSensorDiscovery)) {
    Serial.printf("ERROR: DID NOT PUBLISH SENSOR CONFIG");
  }
  if (!client.publish("homeassistant/sensor/mb/status/config", jsonStatusDiscovery)) {
    Serial.printf("ERROR: DID NOT PUBLISH STATUS CONFIG");
  }
  configPublish = true;
}

// Update device state
bool updateDeviceState(MobiusDevice& device) {
  const int maxRetries = 3;
  for (int i = 0; i < maxRetries; i++) {
    if (device.connect()) {
      uint16_t sceneId = device.getCurrentScene();
      Serial.printf("INFO: SCENE ID FOR DEVICE: %u\n", sceneId);

      if (currState) {
        Serial.printf("INFO: SETTING FEED SCENE FOR DEVICE\n");
        device.setFeedScene();
      } else {
        Serial.printf("INFO: RUNNING SCHEDULE FOR DEVICE\n");
        device.runSchedule();
      }

      device.disconnect();
      Serial.printf("INFO: SUCCESSFULLY UPDATED DEVICE\n");
      return true;
    } else {
      Serial.printf("ERROR: FAILED TO CONNECT TO DEVICE (Try %i)\n", i);
      delay(1000); // Wait for a second before retrying
    }
  }
  Serial.println("ERROR: EXCEEDED MAX RETRIES FOR DEVICE CONNECTION");
  return false;
}

// Publish MQTT message with retry mechanism
bool publishWithRetry(const char* topic, const char* payload, int retries = 3) {
  for (int i = 0; i < retries; i++) {
    if (client.publish(topic, payload)) {
      return true;
    }
    delay(1000);  // Wait for a second before retrying
  }
  return false;
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {};

  WiFi.setHostname("esp32-mc");

  prevState = !currState;
  firstRun = true;
  client.enableDebuggingMessages();
  client.setMaxPacketSize(2048);  // Adjust size as needed
  client.setKeepAlive(60);  // 60 seconds keep-alive interval

  MobiusDevice::init(new ArduinoSerialDeviceEventListener());
  Serial.println("SETUP RUN");
}

void loop() {
  ensureWiFiConnected();
  ensureMQTTConnected();
  client.loop();

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
        Serial.printf("INFO: PUBLISHING STATE\n");
        if (!firstRun && deviceCount > 0) {
          currentDeviceIndex = 0;
          connectTries = 0;
          currentState = HANDLE_DEVICES;
        }

        char cstr[20];
        StaticJsonDocument<100> jsonQtyDev;
        jsonQtyDev["qtydevices"] = deviceCount;
        serializeJson(jsonQtyDev, cstr, sizeof(cstr));
        Serial.printf("INFO: MOBIUS BLE DEVICE COUNT: %i\n", deviceCount);
        if (!publishWithRetry("homeassistant/sensor/mb/state", cstr)) {
          Serial.printf("ERROR: DID NOT PUBLISH DEVICE QUANTITY\n");
        }

        prevState = currState;
        firstRun = false;
      }
      break;

    case HANDLE_DEVICES:
      if (currentDeviceIndex < deviceCount) {
        MobiusDevice& device = deviceBuffer[currentDeviceIndex];
        bool success = updateDeviceState(device);
        currentDeviceIndex++;
        if (currentDeviceIndex >= deviceCount) {
          const char* statusStr = success ? "Success" : "Failed";
          if (!publishWithRetry("homeassistant/sensor/mb/status", statusStr)) {
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
