var captivePortalDebug = false;
var forcedCaptivePortalDebug = false;
var captivePortalActive = false;
var forcedCaptivePortal = false;
var captivePortalNoTimeout = false;
var forcedCaptivePortalNoTimeout = false;
var relaxedSecurity = false; // Flag to control relaxed security mode
var forcedRelaxedSecurity = false; // Enadled by url parameter "relaxedSecurity"

// Global variable to control captive portal test mode
var forceCaptivePortalActive = false; // Flag to indicate url parameters include "forceCaptivePortalActive"
var preferencesDebug = false; // Flag to enable debug output in the console

// Global variables to store the captive portal status
var captivePortalStatus = {
    captivePortalTimeLeft: 0
};

// Global variables to store the previous state
var previousData = {
    forceCaptivePortalActive: false,
    captivePortalActive: false,
    forcedCaptivePortal: false,
    captivePortalDebug: false,
    relaxedSecurity: false,
    forcedCaptivePortalDebug: false,
    captivePortalNoTimeout: false
};

// Global variables to store the CO2 Gadget features supported by the device (selected at compile time)
var features = {
    SUPPORT_BLE: false,
    SUPPORT_BUZZER: false,
    SUPPORT_ESPNOW: false,
    SUPPORT_MDNS: false,
    SUPPORT_MQTT: false,
    SUPPORT_MQTT_DISCOVERY: false,
    SUPPORT_OTA: false,
    SUPPORT_LOW_POWER: false
};

/**
 * Restarts the ESP32 device after user confirmation.
 */
function restartESP32() {
    const isConfirmed = confirm("Are you sure you want to restart the ESP32?");
    if (isConfirmed) {
        if (preferencesDebug) console.log("Restarting ESP32...");
        fetch('/restart', {
            method: 'GET',
            headers: {
                'Content-Type': 'text/plain'
            }
        })
            .then(response => {
                if (response.ok) {
                    console.log('ESP32 restart initiated');
                } else {
                    throw new Error(`HTTP error! Status: ${response.status}`);
                }
            })
            .catch(error => console.error('Error restarting ESP32:', error));
    }
}

/**
 * Adds the 'active' class to the current page link in the navigation bar.
 */
function highlightCurrentPage() {
    const currentPage = window.location.pathname.split("/").pop();
    const navLinks = document.querySelectorAll(".navbar .nav-content a");

    navLinks.forEach(link => {
        if (link.getAttribute("href") === currentPage) {
            link.classList.add("active");
        }
    });
}

function loadFeaturesFromServer() {
    fetch('/getFeaturesAsJson')
        .then(response => response.json())
        .then(data => {
            console.log('Fetching loadFeaturesFromServer successful!');
            // Update DOM with status data
            if (data.BLE !== undefined) {
                features.SUPPORT_BLE = data.BLE;
            } else {
                features.SUPPORT_BLE = false;
            }

            if (data.Buzzer !== undefined) {
                features.SUPPORT_BUZZER = data.Buzzer;
            }
            else {
                features.SUPPORT_BUZZER = false;
            }

            if (data.EspNow !== undefined) {
                features.SUPPORT_ESPNOW = data.EspNow;
            }
            else {
                features.SUPPORT_ESPNOW = false;
            }

            if (data.mDNS !== undefined) {
                features.SUPPORT_MDNS = data.mDNS;
            }
            else {
                features.SUPPORT_MDNS = false;
            }

            if (data.MQTT !== undefined) {
                features.SUPPORT_MQTT = data.MQTT;
            }
            else {
                features.SUPPORT_MQTT = false;
            }

            if (data.MQTTDiscovery !== undefined) {
                features.SUPPORT_MQTT_DISCOVERY = data.MQTTDiscovery;
            }
            else {
                features.SUPPORT_MQTT_DISCOVERY = false;
            }

            if (data.OTA !== undefined) {
                features.SUPPORT_OTA = data.OTA;
            }
            else {
                features.SUPPORT_OTA = false;
            }

            if (data.LowPower !== undefined) {
                features.SUPPORT_LOW_POWER = data.LowPower;
            }
            else {
                features.SUPPORT_LOW_POWER = false;
            }
        }
        )
        .catch(error => console.error('Error fetching features:', error));
}

// Getters

function readBatteryVoltage() {
    return fetch('/readBatteryVoltage')
        .then(response => {
            if (!response.ok) {
                throw new Error('Network response was not ok.');
            }
            return response.text();
        })
        .then(voltage => {
            return voltage;
        })
        .catch(error => {
            console.error('Error fetching battery voltage:', error);
            throw error;
        });
}

function readFreeHeap() {
    return fetch('/getFreeHeap')
        .then(response => {
            if (!response.ok) {
                throw new Error('Network response was not ok.');
            }
            return response.text();
        })
        .then(heap => {
            return heap;
        })
        .catch(error => {
            console.error('Error fetching free heap:', error);
            throw error;
        });
}

function readMinFreeHeap() {
    return fetch('/getMinFreeHeap')
        .then(response => {
            if (!response.ok) {
                throw new Error('Network response was not ok.');
            }
            return response.text();
        })
        .then(heap => {
            return heap;
        })
        .catch(error => {
            console.error('Error fetching min free heap:', error);
            throw error;
        });
}

/**
 * Handles the features data and updates the SUPPORT_* properties accordingly.
 * @param {Object} features - The features data object.
 */
function handleFeaturesData(data) {
    features = data;
    if (captivePortalDebug) console.log('Features:', features);
}

/**
 * Fetches features as JSON from the server and handles the response.
 */
function getFeaturesAsJson() {
    fetch("/getFeaturesAsJson")
        .then(response => {
            if (!response.ok) {
                // if (captivePortalDebug) console.log("Received response:", response);
                console.error("Response not OK:", response.status, response.statusText);
                throw new Error("Network response was not ok " + response.statusText);
            }
            return response.json();
        })
        .then(data => {
            console.log("Received JSON:", data);
            handleFeaturesData(data);
        })
        .catch(error => {
            console.error("Error fetching CO2 Gadget features:", error);
        });
}
