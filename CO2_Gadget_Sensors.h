#ifndef CO2_Gadget_Sensors_h
#define CO2_Gadget_Sensors_h

#include <Sensors.hpp>

bool firstCO2SensorInit = true;
volatile bool pendingCalibration = false;
volatile bool newReadingsAvailable = false;
uint16_t calibrationValue = 415;
uint16_t customCalibrationValue = 415;
bool pendingAmbientPressure = false;
uint16_t ambientPressureValue = 0;
uint16_t altitudeMeters = 600;
bool autoSelfCalibration = false;
float tempOffset = 0.0f;

volatile uint16_t co2 = 0;
volatile uint16_t previousCO2Value = 0;
float temp, tempFahrenheit, hum = 0;
String mainDeviceSelected = "";

String sensorsGetMainDeviceSelected() {
    return mainDeviceSelected;
}

void printSensorsDetected() {
    uint16_t sensors_count = sensors.getSensorsRegisteredCount();
    uint16_t units_count = sensors.getUnitsRegisteredCount();
    Serial.println("-->[SENS]  Sensors detected count\t: " + String(sensors_count));
    Serial.println("-->[SENS]  Sensors units count  \t: " + String(units_count));
    Serial.print("-->[SENS]  Sensors devices names\t: ");
    int i = 0;
    while (sensors.getSensorsRegistered()[i++] != 0) {
        Serial.print(sensors.getSensorName((SENSORS)sensors.getSensorsRegistered()[i - 1]));
        mainDeviceSelected = sensors.getSensorName((SENSORS)sensors.getSensorsRegistered()[i - 1]);
        Serial.print(",");
    }
    Serial.println();
}

void onSensorDataOk() {
    previousCO2Value = co2;
    co2 = sensors.getCO2();
    hum = sensors.getHumidity();
    if (hum == 0.0) hum = sensors.getCO2humi();
    temp = sensors.getTemperature();
    if (temp == 0.0) temp = sensors.getCO2temp();  // TO-DO: temp could be 0.0
    tempFahrenheit = (temp * 1.8 + 32);
    if (!inMenu) {
        Serial.printf("-->[SENS] CO2: %d CO2temp: %.2f CO2humi: %.2f H: %.2f T: %.2f\n", co2, sensors.getCO2temp(), sensors.getCO2humi(), sensors.getHumidity(), sensors.getTemperature());
    }
    newReadingsAvailable = true;
    // Serial.printf("-->[SENS] Free heap: %d\n", ESP.getFreeHeap());
}

void onSensorDataError(const char *msg) { Serial.println(msg); }

// To move into the sensorlib
uint16_t getSCD4xFeatureSet() {
    uint16_t featureSet = 0;
    uint16_t error = 0;
    error = sensors.scd4x.stopPeriodicMeasurement();
    error = sensors.scd4x.getFeatures(featureSet);
    if (error != 0) {
        Serial.println("-->[SENS] SCD4x getFeatures error: " + String(error));
    } else {
        uint8_t typeOfSensor = ((featureSet & 0x1000) >> 12);
        Serial.println("-->[SENS] SCD4x Sensor Type: SCD4" + String(typeOfSensor));
    }
    sensors.scd4x.startPeriodicMeasurement();
    return featureSet;
}

void initSensors() {
    const int8_t None = -1, AUTO = 0, MHZ19 = 4, CM1106 = 5, SENSEAIRS8 = 6, DEMO = 127;
    if (firstCO2SensorInit) {
        Serial.printf("-->[SENS] Using sensorlib v%s Rev:%d\n", CSL_VERSION, CSL_REVISION);
        firstCO2SensorInit = false;
    }

    displayNotification("Init sensors", notifyInfo);

#ifdef ENABLE_PIN
    // Turn On the Sensor (reserved for future use)
    Serial.println("-->[SENS] Turning on sensor..");
    pinMode(ENABLE_PIN, OUTPUT);
    digitalWrite(ENABLE_PIN, ENABLE_PIN_HIGH);
    delay(50);
#endif

// Initialize sensors
#ifdef I2C_SDA &&defined(I2C_SCL)
    Wire.begin(I2C_SDA, I2C_SCL);
#else
    Wire.begin();
#endif

    Serial.println("-->[SENS] Detecting sensors..");

    uint16_t defaultCO2MeasurementInterval = 5;  // TO-DO: Move to preferences
// Breaking change: https://github.com/kike-canaries/canairio_sensorlib/pull/110
// CanAirIO Sensorlib was multipliying sample time by two until rev 340 (inclusive). Adjust to avoid need for recalibration.
#ifdef CSL_REVISION  // CanAirIO Sensorlib Revision > 340 (341 where CSL_REVISION was included)
    if (sensors.getLibraryRevision() > 340) {
        sensors.setSampleTime(defaultCO2MeasurementInterval * 2);
    } else {
        sensors.setSampleTime(defaultCO2MeasurementInterval);
    }
#else
    sensors.setSampleTime(defaultCO2MeasurementInterval);
#endif

    sensors.setOnDataCallBack(&onSensorDataOk);      // all data read callback
    sensors.setOnErrorCallBack(&onSensorDataError);  // [optional] error callback
    sensors.setDebugMode(debugSensors);              // [optional] debug mode
    sensors.setTempOffset(tempOffset);
    sensors.setCO2AltitudeOffset(altitudeMeters);
    // sensors.setAutoSelfCalibration(false); // TO-DO: Implement in CanAirIO Sensors Lib

    // setBLEHistoryInterval(sampleInterval);

    Serial.printf("-->[SENS] Selected CO2 Sensor: %d\n", selectedCO2Sensor);

    if (selectedCO2Sensor == AUTO) {
        Serial.println("-->[SENS] Trying to init CO2 sensor: AutoSensor (I2C)");
        sensors.detectI2COnly(true);
        sensors.init();
    } else if (selectedCO2Sensor == MHZ19) {
        Serial.println("-->[SENS] Trying to init CO2 sensor: MHZ19(A/B/C/D)");
        sensors.detectI2COnly(false);
        sensors.init(MHZ19);
    } else if (selectedCO2Sensor == CM1106) {
        Serial.println("-->[SENS] Trying to init CO2 sensor: CM1106");
        sensors.detectI2COnly(false);
        sensors.init(CM1106);
    } else if (selectedCO2Sensor == SENSEAIRS8) {
        Serial.println("-->[SENS] Trying to init CO2 sensor: SENSEAIRS8");
        sensors.detectI2COnly(false);
        sensors.init(SENSEAIRS8);
    }

    printSensorsDetected();

    if (!sensorsGetMainDeviceSelected().isEmpty()) {
        Serial.println("-->[SENS] Sensor configured: " + sensorsGetMainDeviceSelected());

        // Temporary getFeatureSet() for SCD4x. To be moved into the sensorlib
        if (sensorsGetMainDeviceSelected() == "SCD4X") {
            Serial.println("-->[SENS] SCD4x Feature Set: " + String(getSCD4xFeatureSet()));
        }
    }
}

void sensorsLoop() {
    if (isDownloadingBLE) return;
    if (!buzzerBeeping) {
        sensors.loop();
    }
}

#endif  // CO2_Gadget_Sensors_h