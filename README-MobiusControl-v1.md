# MobiusControl-v1.ino

This code is an implementation for an ESP32 microcontroller that integrates with Home Assistant using MQTT for communication and manages Bluetooth Low Energy (BLE) devices using a custom Mobius protocol. Here’s a breakdown of its functions and features:
Libraries and MQTT Client Setup

    Libraries Imported:
        ESP32_MobiusBLE.h: Custom library for managing Mobius BLE devices.
        ArduinoSerialDeviceEventListener.h: Custom event listener for Arduino serial devices.
        EspMQTTClient.h: For MQTT client functionalities.
        WiFi.h: For WiFi connection management.
        ArduinoJson.h: For JSON parsing and creation.

    MQTT Client Object:
        Configures the MQTT client with WiFi credentials, MQTT broker address, and credentials.
        EspMQTTClient client: Initialized with parameters for connecting to WiFi and the MQTT broker.

JSON Discovery Messages

    Discovery Messages:
        jsonSwitchDiscovery, jsonSensorDiscovery, jsonStatusDiscovery: Define the configuration messages for Home Assistant to discover and configure the switch and sensor devices.

State Variables and Configuration

    State Variables:
        prevState, currState, configPublish, firstRun: Track the state of the system and devices.
        currentState: Enum to manage different states (IDLE, SCANNING, PUBLISH_CONFIG, PUBLISH_STATE, HANDLE_DEVICES).

Callback and Connection Functions

    MQTT Connection Callback:
        onConnectionEstablished(): Subscribes to a topic and handles messages to change the device state based on MQTT commands.

    WiFi and MQTT Connection Management:
        ensureWiFiConnected(): Ensures the device is connected to WiFi, with retry mechanism.
        ensureMQTTConnected(): Ensures the MQTT client is connected to the broker, with retry mechanism.

Device Management Functions

    Scanning for Devices:
        scanForDevices(): Scans for Mobius BLE devices with retries, stores them in a buffer.

    Publishing Configuration to MQTT:
        publishConfigs(): Publishes configuration messages for devices to MQTT topics.

    Updating Device State:
        updateDeviceState(MobiusDevice& device): Connects to a device, updates its state based on the current mode, and disconnects with a retry mechanism.

    Publishing MQTT Messages with Retries:
        publishWithRetry(const char* topic, const char* payload, int retries): Publishes messages to MQTT with retries.

Main Functions (Setup and Loop)

    Setup Function:
        setup(): Initializes the serial communication, sets the hostname, configures MQTT client, and initializes the MobiusDevice library.

    Main Loop Function:
        loop(): Manages different states of the device using a state machine:
            IDLE State: Waits for the scan interval to elapse.
            SCANNING State: Scans for devices and transitions to publishing configurations.
            PUBLISH_CONFIG State: Publishes configuration if not already done and transitions to publish state.
            PUBLISH_STATE State: Publishes device state to MQTT and transitions to handle devices.
            HANDLE_DEVICES State: Updates each device's state and publishes status to MQTT.

Summary

    This code provides a robust system for managing and integrating BLE devices with Home Assistant using MQTT.
    It handles WiFi and MQTT connection management, device scanning, and state updates.
    It uses a state machine to manage different tasks and ensures reliable communication with retries and error handling.

