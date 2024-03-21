#ifndef CO2_Gadget_DeepSleep_h
#define CO2_Gadget_DeepSleep_h

#include "esp32-hal-gpio.h"

/*****************************************************************************************************/
/*********                                                                                   *********/
/*********                                    DEEP SLEEP                                     *********/
/*********                                                                                   *********/
/*****************************************************************************************************/

#include <Sensors.hpp>

#define DEEP_SLEEP_DEBUG

// CO2 sensors enum
// typedef enum {
//     CO2Sensor_NONE = -1,
//     CO2Sensor_SCD30 = 1,
//     CO2Sensor_SCD40 = 2,
//     CO2Sensor_SCD41 = 3,
//     CO2Sensor_MHZ19 = 4,
//     CO2Sensor_CM1106 = 5,
//     CO2Sensor_CM1106SL_NS = 6,
//     CO2Sensor_SENSEAIRS8 = 7,
//     CO2Sensor_DEMO = 127
// } CO2SENSORS_t;

String getLowPowerModeName(uint16_t mode) {
    switch (mode) {
        case HIGH_PERFORMANCE:
            return "HIGH_PERFORMANCE";
        case BASIC_LOWPOWER:
            return "BASIC_LOWPOWER";
        case MEDIUM_LOWPOWER:
            return "MEDIUM_LOWPOWER";
        case MAXIMUM_LOWPOWER:
            return "MAXIMUM_LOWPOWER";
        default:
            return "UNKNOWN";
    }
}

String getDeepSleepDataCo2SensorName() {
    switch (deepSleepData.co2Sensor) {
        case CO2Sensor_NONE:
            return "NONE";
        case CO2Sensor_SCD30:
            return "SCD30";
        case CO2Sensor_SCD40:
            return "SCD40";
        case CO2Sensor_SCD41:
            return "SCD41";
        case CO2Sensor_MHZ19:
            return "MHZ19";
        case CO2Sensor_CM1106:
            return "CM1106";
        case CO2Sensor_CM1106SL_NS:
            return "CM1106SL_NS";
        case CO2Sensor_SENSEAIRS8:
            return "SENSEAIRS8";
        case CO2Sensor_DEMO:
            return "DEMO";
        default:
            return "UNKNOWN";
    }
}

void SetTimeToDeepSleep(uint16_t time) {
    deepSleepData.timeSleeping = time;
#ifdef DEEP_SLEEP_DEBUG
    Serial.println("-->[DEEP] Time to be in deep sleep: " + String(deepSleepData.timeSleeping) + " seconds");
#endif
}

String getWakeupCause() {
    switch (esp_sleep_get_wakeup_cause()) {
        case ESP_SLEEP_WAKEUP_UNDEFINED:
            return "Undefined";
        case ESP_SLEEP_WAKEUP_ALL:
            return "All";
        case ESP_SLEEP_WAKEUP_EXT0:
            return "External signal using RTC_IO";
        case ESP_SLEEP_WAKEUP_EXT1:
            return "External signal using RTC_CNTL";
        case ESP_SLEEP_WAKEUP_TIMER:
            return "Timer";
        case ESP_SLEEP_WAKEUP_TOUCHPAD:
            return "Touchpad";
        case ESP_SLEEP_WAKEUP_ULP:
            return "ULP program";
        case ESP_SLEEP_WAKEUP_GPIO:
            return "GPIO";
        case ESP_SLEEP_WAKEUP_UART:
            return "UART";
        case ESP_SLEEP_WAKEUP_WIFI:
            return "WIFI";
        case ESP_SLEEP_WAKEUP_COCPU:
            return "COCPU int";
        case ESP_SLEEP_WAKEUP_COCPU_TRAP_TRIG:
            return "COCPU crash";
        case ESP_SLEEP_WAKEUP_BT:
            return "BT";
        default:
            return "Unknown";
    }
}

String getResetReason() {
    switch (esp_reset_reason()) {
        case ESP_RST_UNKNOWN:
            return "Unknown reset reason";
        case ESP_RST_POWERON:
            return "Power on reset";
        case ESP_RST_EXT:
            return "External reset";
        case ESP_RST_SW:
            return "Software reset";
        case ESP_RST_PANIC:
            return "Software reset due to exception/panic";
        case ESP_RST_INT_WDT:
            return "Reset due to interrupt watchdog";
        case ESP_RST_TASK_WDT:
            return "Reset due to task watchdog";
        case ESP_RST_WDT:
            return "Reset due to other watchdogs";
        case ESP_RST_DEEPSLEEP:
            return "Reset after exiting deep sleep mode";
        case ESP_RST_BROWNOUT:
            return "Brownout reset";
        case ESP_RST_SDIO:
            return "Reset over SDIO";
        default:
            return "Unknown reset reason";
    }
}

void printResetReason() {
#ifdef DEEP_SLEEP_DEBUG
    Serial.println("-->[DEEP] Reason for reset: " + getResetReason());
#endif
}

void printRTCMemoryEnter() {
#ifdef DEEP_SLEEP_DEBUG2
    Serial.println("-->[DEEP][ENTER] lowPowerMode: " + String(deepSleepData.lowPowerMode));
    Serial.println("-->[DEEP][ENTER] co2Sensor: " + String(deepSleepData.co2Sensor));
    Serial.println("-->[DEEP][ENTER] waitToGoDeepSleepOn1stBoot: " + String(deepSleepData.waitToGoDeepSleepOn1stBoot));
    Serial.println("-->[DEEP][ENTER] timeSleeping: " + String(deepSleepData.timeSleeping));
    Serial.println("-->[DEEP][ENTER] gpioConfig: " + String(deepSleepData.gpioConfig));
    Serial.println("-->[DEEP][ENTER] waitingForDataReady: " + String(deepSleepData.waitingForDataReady));
    Serial.println("-->[DEEP][ENTER] cyclesToWiFiConnect: " + String(deepSleepData.cyclesToWiFiConnect));
    Serial.println("-->[DEEP][ENTER] cyclesToRedrawDisplay: " + String(deepSleepData.cyclesToRedrawDisplay));
    Serial.println("-->[DEEP][ENTER] lastCO2Value: " + String(deepSleepData.lastCO2Value));
    Serial.println("-->[DEEP][ENTER] lastTemperatureValue: " + String(deepSleepData.lastTemperatureValue));
    Serial.println("-->[DEEP][ENTER] lastHumidityValue: " + String(deepSleepData.lastHumidityValue));
    Serial.println("-->[DEEP][ENTER] activeWifiOnWake: " + String(deepSleepData.activeWifiOnWake));
    Serial.println("-->[DEEP][ENTER] sendMQTTOnWake: " + String(deepSleepData.sendMQTTOnWake));
    Serial.println("-->[DEEP][ENTER] sendESPNowOnWake: " + String(deepSleepData.sendESPNowOnWake));
    Serial.println("-->[DEEP][ENTER] displayOnWake: " + String(deepSleepData.displayOnWake));
#endif
}

void printRTCMemoryExit() {
#ifdef DEEP_SLEEP_DEBUG2
    Serial.println("-->[DEEP][EXIT] lowPowerMode: " + String(deepSleepData.lowPowerMode));
    Serial.println("-->[DEEP][EXIT] co2Sensor: " + String(deepSleepData.co2Sensor));
    Serial.println("-->[DEEP][EXIT] waitToGoDeepSleepOn1stBoot: " + String(deepSleepData.waitToGoDeepSleepOn1stBoot));
    Serial.println("-->[DEEP][EXIT] timeSleeping: " + String(deepSleepData.timeSleeping));
    Serial.println("-->[DEEP][EXIT] gpioConfig: " + String(deepSleepData.gpioConfig));
    Serial.println("-->[DEEP][EXIT] waitingForDataReady: " + String(deepSleepData.waitingForDataReady));
    Serial.println("-->[DEEP][EXIT] cyclesToWiFiConnect: " + String(deepSleepData.cyclesToWiFiConnect));
    Serial.println("-->[DEEP][EXIT] cyclesToRedrawDisplay: " + String(deepSleepData.cyclesToRedrawDisplay));
    Serial.println("-->[DEEP][EXIT] lastCO2Value: " + String(deepSleepData.lastCO2Value));
    Serial.println("-->[DEEP][EXIT] lastTemperatureValue: " + String(deepSleepData.lastTemperatureValue));
    Serial.println("-->[DEEP][EXIT] lastHumidityValue: " + String(deepSleepData.lastHumidityValue));
    Serial.println("-->[DEEP][EXIT] activeWifiOnWake: " + String(deepSleepData.activeWifiOnWake));
    Serial.println("-->[DEEP][EXIT] sendMQTTOnWake: " + String(deepSleepData.sendMQTTOnWake));
    Serial.println("-->[DEEP][EXIT] sendESPNowOnWake: " + String(deepSleepData.sendESPNowOnWake));
    Serial.println("-->[DEEP][EXIT] displayOnWake: " + String(deepSleepData.displayOnWake));
#endif
}

void toDeepSleep() {
#ifdef TIMEDEBUG
    Serial.println("-->[DEEP] Time awake: " + String(timerAwake.read()) + " ms");
#endif
    // deepSleepData.lowPowerMode = 2;
    Serial.println("");
    Serial.println("");
    Serial.println("-->************************************************************************************************************************************************");
    Serial.println("-->[DEEP] Going into deep sleep for " + String(deepSleepData.timeSleeping) + " seconds with LowPowerMode: " + String(deepSleepData.lowPowerMode) + " (" + getLowPowerModeName(deepSleepData.lowPowerMode) + ")");
    Serial.println("-->************************************************************************************************************************************************");
    Serial.println("");
    printRTCMemoryEnter();
    Serial.flush();

    // Experimental: Turn off green LED and display on S3 board
    // #if defined(CONFIG_IDF_TARGET_ESP32S3)
    // digitalWrite(TFT_POWER_ON_BATTERY, LOW);
    // #endif

    // Pull up pin 13 to put flash memory into deep sleep
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);
    // gpio_hold_en(gpio_num_t(13));

    esp_deep_sleep_disable_rom_logging();
#ifdef BTN_WAKEUP
    esp_sleep_enable_ext0_wakeup(static_cast<gpio_num_t>(BTN_WAKEUP), BTN_WAKEUP_ON);  // 1 = High, 0 = Low
#else
    if (BTN_DWN != -1) {
        esp_sleep_enable_ext0_wakeup(static_cast<gpio_num_t>(BTN_DWN), LOW);  // 1 = High, 0 = Low
    } else if (BTN_UP != -1) {
        esp_sleep_enable_ext0_wakeup(static_cast<gpio_num_t>(BTN_UP), LOW);  // 1 = High, 0 = Low
    }
#endif
    esp_sleep_enable_timer_wakeup(deepSleepData.timeSleeping * 1000000);
    delay(5);
    gpio_deep_sleep_hold_en();
    esp_deep_sleep_start();
}

bool isDataReadySCD4x() {
    bool dataReadyFlag = false;
    uint16_t error = 0;
    error = sensors.scd4x.getDataReadyFlag(dataReadyFlag);
    if (error != 0) {
        Serial.println("-->[DEEP] SCD4X isDataReadySCD4x() error: " + String(error));
    }
    return dataReadyFlag;
}

void doDeepSleepMQTTConnect() {
    if (WiFi.status() == WL_CONNECTED) {
        // Serial.printf("-->[DEEP] Initializing MQTT to broker IP: %s\n", mqttBroker.c_str());
        Serial.println("-->[DEEP] Initializing MQTT tobroker IP: " + mqttBroker);
        mqttClient.setServer(mqttBroker.c_str(), 1883);
        mqttClient.setCallback(callbackMQTT);
        mqttClient.setBufferSize(1024);
        if (mqttClient.connect((mqttClientId).c_str(), (mqttUser).c_str(), (mqttPass).c_str())) {
            // Serial.printf("connected\n");
            Serial.println("-->[DEEP] MQTT connected.");
            Serial.print("-->[DEEP] rootTopic: ");
            Serial.println(rootTopic);
        }
    }
}

void doDeepSleepWiFiConnect() {
    initPreferences();
    if (deepSleepData.activeWifiOnWake) {
        connectToWiFi();
    }

#ifdef SUPPORT_ESPNOW
    if (deepSleepData.sendESPNowOnWake) {
        initESPNow();
    }
#endif
#ifdef SUPPORT_MQTT
    if (deepSleepData.sendMQTTOnWake) {
        doDeepSleepMQTTConnect();
    }
#endif
    deepSleepData.cyclesToWiFiConnect = deepSleepWiFiConnectEach;
}

void displayFromDeepSleep(bool forceRedraw = false) {
#ifdef SUPPORT_EINK
    initDisplayFromDeepSleep(forceRedraw);
    displayShowValues();
#endif
}

void reInitI2C() {
    // Re-Initialize I2C bus
    if (!Wire.available()) {
#if defined(CUSTOM_I2C_SDA) && defined(CUSTOM_I2C_SCL)
        Wire.begin(CUSTOM_I2C_SDA, CUSTOM_I2C_SCL);
#else
        Wire.begin();
#endif
        delay(10);
    }
}

void cm1106HandleFromDeepSleep() {
    pinMode(CM1106_ENABLE_PIN, OUTPUT);
    digitalWrite(CM1106_ENABLE_PIN, HIGH);
    pinMode(CM1106_READY_PIN, INPUT);
    Serial.print("-->[DEEP] Waking up from deep sleep. Handling CM1106 ");
#if defined(UART_RX_GPIO) && defined(UART_TX_GPIO)
    sensors.init(CM1106, UART_RX_GPIO, UART_TX_GPIO);
#else
    sensors.init(CM1106);
#endif

    while (digitalRead(CM1106_READY_PIN) == LOW) {
        Serial.print("+");
        delay(10);
    }

    Serial.println("");

    co2 = sensors.cm1106->get_co2();
    deepSleepData.lastCO2Value = co2;
    // Serial.printf("-->[DEEP] CO2 value: %d ppm\n", co2);
    Serial.println("-->[DEEP] CO2 value: " + String(co2) + " ppm");
    displayFromDeepSleep(deepSleepData.cyclesToRedrawDisplay == 0);
}

void scd41HandleFromDeepSleep(bool blockingMode = true) {
    static bool initialized = false;
    unsigned long previousMillis = 0;
    uint16_t error = 0;
    uint16_t co2value = 0;
    float temperature = 0;
    float humidity = 0;

    Serial.println("-->[DEEP] scd41HandleFromDeepSleep()");

    if (!initialized) {
        reInitI2C();
        sensors.scd4x.begin(Wire);
        initialized = true;
    }

    if ((!blockingMode) && (!isDataReadySCD4x())) return;

    error = sensors.scd4x.measureSingleShot(true);
    if (error != 0) {
        Serial.println("-->[DEEP] Waking up from deep sleep. measureSingleShot() error: " + String(error));
    }
    while (!isDataReadySCD4x()) {
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillis >= 1000) {
            previousMillis = currentMillis;
            Serial.print("+");
            delay(10);
        }
    }
    error = sensors.scd4x.readMeasurement(co2value, temperature, humidity);
    co2 = co2value;
    temp = temperature;
    hum = humidity;
    deepSleepData.lastCO2Value = co2;
    deepSleepData.lastTemperatureValue = temp;
    deepSleepData.lastHumidityValue = hum;
    if (error != 0) {
        // Serial.printf("-->[DEEP] Waking up from deep sleep. readMeasurement() error: %d\n", error);
        Serial.println("-->[DEEP] Waking up from deep sleep. readMeasurement() error " + String(error));

    } else {
        displayFromDeepSleep(deepSleepData.cyclesToRedrawDisplay == 0);
    }
}

void scd40HandleFromDeepSleep(bool blockingMode = true) {
    static bool initialized = false;
    unsigned long previousMillis = 0;
    uint16_t error = 0;
    uint16_t co2value = 0;
    float temperature = 0;
    float humidity = 0;

    Serial.println("-->[DEEP] scd40HandleFromDeepSleep()");

    if (!initialized) {
        reInitI2C();
        sensors.scd4x.begin(Wire);
        initialized = true;
    }

    if ((!blockingMode) && (!isDataReadySCD4x())) return;
    while (!isDataReadySCD4x()) {
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillis >= 1000) {
            previousMillis = currentMillis;
            Serial.print("+");
            delay(10);
        }
    }
    error = sensors.scd4x.readMeasurement(co2value, temperature, humidity);
    co2 = co2value;
    temp = temperature;
    hum = humidity;
    deepSleepData.lastCO2Value = co2;
    deepSleepData.lastTemperatureValue = temp;
    deepSleepData.lastHumidityValue = hum;
    if (error != 0) {
        Serial.println("-->[DEEP] Waking up from deep sleep. readMeasurement() error " + String(error));

    } else {
        displayFromDeepSleep(deepSleepData.cyclesToRedrawDisplay == 0);
    }
}

void scd30HandleFromDeepSleepNEW() {
    Adafruit_SCD30 scd30;
    unsigned long previousMillis = 0;

    // if (Wire.available()) {
    //     Serial.println("-->[DEEP] Wire is available");
    // } else {
    //     Serial.println("-->[DEEP] Wire is NOT available");
    reInitI2C();
    // }

    // // Try to initialize!
    if (!scd30.begin()) {
        Serial.println("[DEEP][ERROR] Failed to find SCD30 sensor");
        // while (1) {
        //     delay(10);
        // }
    }

    if (!scd30.setMeasurementInterval(measurementInterval)) {
        Serial.println("[DEEP][SCD30][ERROR] Failed to set measurement interval");
        // while (1) {
        //     delay(10);
        // }
    }

    // While the setting is recorded, it is not immediately available to be read.
    delay(200);

    Serial.println("-->[DEEP][SCD30] Found! Starting continuous measurement");
    uint16_t actualMeasurementInterval = scd30.getMeasurementInterval();
    Serial.println("-->[DEEP][SCD30] Measurement Interval: " + String(actualMeasurementInterval) + " seconds");

    if (!scd30.startContinuousMeasurement(0)) {
        delay(50);
        if (!scd30.startContinuousMeasurement(0)) {
            Serial.println("[DEEP][SCD30][ERROR] Failed to start continuous measurement");
            // while (1) {
            //     delay(10);
            // }
        }
    }

    delay(measurementInterval * 1000 + 100);

    if (!scd30.dataReady()) {
#ifdef DEEP_SLEEP_DEBUG
        Serial.println("-->[DEEP][SCD30] Waiting for data from sensor");
#endif
        while (!scd30.dataReady()) {
            unsigned long currentMillis = millis();
            if (currentMillis - previousMillis >= 1000) {
                previousMillis = currentMillis;
#ifdef DEEP_SLEEP_DEBUG
                Serial.print("+");
#endif
                delay(100);
            }
        }
#ifdef DEEP_SLEEP_DEBUG
        Serial.println("");
#endif
    }

    if (!scd30.read()) {
        Serial.println("-->[DEEP][SCD30][ERROR] Error reading data from sensor");
        return;
    }

    Serial.println("-->[DEEP][SCD30] CO2: " + String(scd30.CO2, 3) + " ppm");
    if (scd30.CO2 > 0) {
        co2 = scd30.CO2;
        deepSleepData.lastCO2Value = co2;
    }

    Serial.println("-->[DEEP][SCD30] Temperature: " + String(scd30.temperature) + " degrees C");
    if (scd30.temperature > 0) {
        temp = scd30.temperature;
        deepSleepData.lastTemperatureValue = temp;
    }

    Serial.println("-->[DEEP][SCD30] Relative Humidity: " + String(scd30.relative_humidity) + " %");
    if (scd30.relative_humidity > 0) {
        hum = scd30.relative_humidity;
        deepSleepData.lastHumidityValue = hum;
    }

    Serial.println("-->[DEEP][SCD30] Stopping continuous measurement");
    scd30.stopContinuousMeasurement();
}

void scd30HandleFromDeepSleepOLD() {
    unsigned long previousMillis = 0;

    reInitI2C();

    // sensors.setOnDataCallBack(&onSensorDataOk);      // all data read callback
    // sensors.setOnErrorCallBack(&onSensorDataError);  // [optional] error callback
    // sensors.setDebugMode(true);
    // sensors.setSampleTime(2);
    // sensors.detectI2COnly(true);

    if (!sensors.scd30.begin() && !sensors.scd30.begin(SCD30_I2CADDR_DEFAULT, &Wire1, SCD30_CHIP_ID)) {
        Serial.println("-->[DEEP][SCD30][ERROR] Failed to find SCD30 sensor");
        return;
    }
#ifdef DEEP_SLEEP_DEBUG
    Serial.println("-->[DEEP][SCD30] SCD30 is not fully supported in Low Power Mode (yet)");
#endif

    // Drop first measurement
    while (!sensors.scd30.dataReady())
        delay(10);
    sensors.scd30.read();

    if (!sensors.scd30.dataReady()) {
#ifdef DEEP_SLEEP_DEBUG
        Serial.println("-->[DEEP][SCD30] Waiting for data from sensor");
#endif
        while (!sensors.scd30.dataReady()) {
            unsigned long currentMillis = millis();
            if (currentMillis - previousMillis >= 1000) {
                previousMillis = currentMillis;
#ifdef DEEP_SLEEP_DEBUG
                Serial.print("+");
#endif
                delay(10);
            }
        }
#ifdef DEEP_SLEEP_DEBUG
        Serial.println("");
#endif
    }

    if (!sensors.scd30.read()) {
        Serial.println("-->[DEEP][SCD30][ERROR] Error reading data from sensor");
        return;
    } else {
        Serial.println("-->[DEEP][SCD30] Data read from sensor");
        uint16_t tCO2 = int(sensors.scd30.CO2);  // we need temp var, without it override CO2
        if (tCO2 > 0) {
            co2 = tCO2;
            hum = sensors.scd30.relative_humidity;
            temp = sensors.scd30.temperature;
        }
#ifdef DEEP_SLEEP_DEBUG
        Serial.println("-->[DEEP][SCD30] Data: CO2: " + String(tCO2) + " ppm, Temp: " + String(sensors.scd30.temperature) + " °C, Hum: " + String(sensors.scd30.relative_humidity) + " %");
#endif
        displayFromDeepSleep(deepSleepData.cyclesToRedrawDisplay == 0);
    }
}

void handleLowPowerSensors() {
    bool blockingMode = true;
    if ((deepSleepEnabled) && (interactiveMode)) blockingMode = false;
    if (deepSleepData.co2Sensor == static_cast<CO2SENSORS_t>(CO2Sensor_SCD30)) {
#ifdef DEEP_SLEEP_DEBUG
        Serial.println("-->[DEEP][SCD30] Waking up from deep sleep. Handling SCD30");
#endif
        // scd30HandleFromDeepSleepNEW();
        scd30HandleFromDeepSleepOLD();

    } else if (deepSleepData.co2Sensor == static_cast<CO2SENSORS_t>(CO2Sensor_CM1106SL_NS)) {
#ifdef DEEP_SLEEP_DEBUG
        Serial.println("-->[DEEP][CM1106SL-NS] Waking up from deep sleep. Handling CM1106SL_NS");
#endif
        cm1106HandleFromDeepSleep();
    } else if (deepSleepData.co2Sensor == static_cast<CO2SENSORS_t>(CO2Sensor_SCD41)) {
#ifdef DEEP_SLEEP_DEBUG
        Serial.println("-->[DEEP][SCD41] Waking up from deep sleep. Handling SCD41");
        scd41HandleFromDeepSleep(blockingMode);
#endif
    } else if (deepSleepData.co2Sensor == static_cast<CO2SENSORS_t>(CO2Sensor_SCD40)) {
#ifdef DEEP_SLEEP_DEBUG
        Serial.println("-->[DEEP][SCD40] Waking up from deep sleep. Handling SCD40");
        scd40HandleFromDeepSleep(blockingMode);
#endif
    } else {
        Serial.println("-->[DEEP][ERROR] deepSleepData.co2Sensor: Unknown");
        sensors.init();
    }
}

void fromDeepSleepTimer() {
    --deepSleepData.cyclesToWiFiConnect;
    --deepSleepData.cyclesToRedrawDisplay;
#if defined(DEEP_SLEEP_DEBUG) && defined(SUPPORT_EINK)
    Serial.println("-->[DEEP] Cycles left to redraw display: " + String(deepSleepData.cyclesToRedrawDisplay));
    Serial.println("-->[DEEP] Cycles left to connect to WiFi: " + String(deepSleepData.cyclesToWiFiConnect));
#endif

    // Set lowPowerMode using the correct type
    // sensors.setLowPowerMode(static_cast<LowPowerMode>(deepSleepData.lowPowerMode));
    // gpioConfig;

    switch (deepSleepData.lowPowerMode) {
        case BASIC_LOWPOWER:
#ifdef DEEP_SLEEP_DEBUG
            Serial.println("-->[DEEP] Waking up from deep sleep. LowPowerMode: BASIC_LOWPOWER");
#endif
            break;

        case MEDIUM_LOWPOWER:
#ifdef DEEP_SLEEP_DEBUG
            Serial.println("-->[DEEP] Waking up from deep sleep. LowPowerMode: MEDIUM_LOWPOWER");
#endif
            initBattery();
            batteryLoop();

            if (deepSleepData.cyclesToWiFiConnect == 0) {
                doDeepSleepWiFiConnect();
            }

            handleLowPowerSensors();

#ifdef DEEP_SLEEP_DEBUG
            Serial.println("-->[DEEP] CO2: " + String(co2) + " CO2temp: " + String(temp) + " CO2humi: " + String(hum));
#endif

#ifdef SUPPORT_MQTT
            if ((deepSleepData.sendMQTTOnWake) && (WiFi.status() == WL_CONNECTED)) {
                Serial.println("-->[DEEP] MQTT connected. Publishing measurements.");
                publishMQTT(true);
            }
#endif
            break;
        case MAXIMUM_LOWPOWER:
            break;
    }
#ifdef TIMEDEBUG
    Serial.println("-->[DEEP] Time awake: " + String(timerAwake.read()) + " ms");
#endif
    Serial.flush();
}

void fromDeepSleep() {
    esp_sleep_wakeup_cause_t wakeupCause;
    wakeupCause = esp_sleep_get_wakeup_cause();
#ifdef DEEP_SLEEP_DEBUG
    Serial.println("-->[STUP] Initializing from deep sleep mode working with sensor (" + String(deepSleepData.co2Sensor) + "): " + getDeepSleepDataCo2SensorName());
#endif
#ifdef DEEP_SLEEP_DEBUG
    Serial.println("");
    Serial.println("");
#endif
    switch (wakeupCause) {
        case ESP_SLEEP_WAKEUP_EXT0:
#ifdef DEEP_SLEEP_DEBUG
            Serial.println("-->[DEEP] Wakeup caused by external signal using RTC_IO");
#endif
            break;
        case ESP_SLEEP_WAKEUP_EXT1:
#ifdef DEEP_SLEEP_DEBUG
            Serial.println("-->[DEEP] Wakeup caused by external signal using RTC_CNTL");
            interactiveMode = true;
#endif
            break;
        case ESP_SLEEP_WAKEUP_TIMER:
#ifdef DEEP_SLEEP_DEBUG
            Serial.println("-->[DEEP] Wakeup caused by timer");
#endif
            fromDeepSleepTimer();
            turnOffDisplay();
            displaySleep(true);
            toDeepSleep();
            break;
        case ESP_SLEEP_WAKEUP_TOUCHPAD:
#ifdef DEEP_SLEEP_DEBUG
            Serial.println("-->[DEEP] Wakeup caused by touchpad");
#endif
            break;
        case ESP_SLEEP_WAKEUP_ULP:
#ifdef DEEP_SLEEP_DEBUG
            Serial.println("-->[DEEP] Wakeup caused by ULP program");
#endif
            break;
        default:
            Serial.println("-->[DEEP] Wakeup was not caused by deep sleep. Reason: " + String(wakeupCause));
            break;
    }
}

void deepSleepLoop() {
    // Variable to store the last time Serial.print was called
    static unsigned long lastSerialPrintTime = 0;
    static unsigned long lastDotPrintTime = 0;

    // if (deepSleepData.lowPowerMode == HIGH_PERFORMANCE) return;
    if (!deepSleepEnabled) return;

    // Serial.println("-->[DEEP] inMenu: " + String(inMenu));

    if (millis() - lastSerialPrintTime >= 5000) {  // Check if some time has passed since the last print
        // Serial.println("-->[DEEP] deepSleepData.waitToGoDeepSleepOn1stBoot: " + String(deepSleepData.waitToGoDeepSleepOn1stBoot * 1000));
        // Serial.println("-->[DEEP] startTimerToDeepSleep: " + String(startTimerToDeepSleep));

        Serial.println("-->[DEEP] Waiting to go to deep sleep in (seconds): " + String((deepSleepData.waitToGoDeepSleepOn1stBoot * 1000 - (millis() - startTimerToDeepSleep)) / 1000) + " seconds");
        // Serial.println("-->[DEEP] startTimerToDeepSleep (ms): " + String(startTimerToDeepSleep) + " deepSleepData.waitToGoDeepSleepOn1stBoot (ms): " + String(deepSleepData.waitToGoDeepSleepOn1stBoot * 1000) + " Now: " + String(millis()));
        lastSerialPrintTime = millis();  // Update last print time
    }

    if ((millis() - lastDotPrintTime >= 1000)) {
        Serial.print(".");            // Print a dot from time to time to show that the device is alive
        lastDotPrintTime = millis();  // Update last print time
    }

    if (inMenu) {
        startTimerToDeepSleep = millis();  // If user enters the menu, reset the timer to go to deep sleep while it's on menu
        // Check if at least one second has passed since the last print
        if (millis() - lastSerialPrintTime >= 5000) {
            // Serial.println("-->[DEEP] startTimerToDeepSleep: " + String(startTimerToDeepSleep) + " deepSleepData.waitToGoDeepSleepOn1stBoot (ms): " + String(deepSleepData.waitToGoDeepSleepOn1stBoot)*1000 + "Now: " + String(millis()));
            Serial.println("-->[DEEP] (inMenu=TRUE) Waiting to go to deep sleep in: " + String((deepSleepData.waitToGoDeepSleepOn1stBoot * 1000 - (millis() - startTimerToDeepSleep)) / 5000) + " seconds");
            lastSerialPrintTime = millis();  // Update last print time
        }
        if ((millis() - lastDotPrintTime >= 1000)) {
            Serial.print(".");            // Print a dot every loop to show that the device is alive
            lastDotPrintTime = millis();  // Update last print time
        }
    } else {
        // Check if at least one second has passed since the last print
        if (millis() - lastSerialPrintTime >= 5000) {
            // Serial.println("-->[DEEP] startTimerToDeepSleep: " + String(startTimerToDeepSleep) + " deepSleepData.waitToGoDeepSleepOn1stBoot: " + String(deepSleepData.waitToGoDeepSleepOn1stBoot) + "Now: " + String(millis()));
            Serial.println("-->[DEEP] (inMenu=FALSE) Waiting to go to deep sleep in: " + String((deepSleepData.waitToGoDeepSleepOn1stBoot * 1000 - (millis() - startTimerToDeepSleep)) / 5000) + " seconds");
            lastSerialPrintTime = millis();  // Update last print time
        }

        if (millis() - startTimerToDeepSleep >= deepSleepData.waitToGoDeepSleepOn1stBoot * 1000) {
            turnOffDisplay();
            displaySleep(true);
            deepSleepData.lowPowerMode = MEDIUM_LOWPOWER;
            toDeepSleep();
        }
    }
}

#endif  // CO2_Gadget_DeepSleep_h