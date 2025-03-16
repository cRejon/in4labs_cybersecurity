#include <ArduinoBLE.h> // Include BLE library
#include <DHT.h>        // Include temperature sensor library

// Pin configuration
#define DATA_PIN 2      // DHT sensor data pin
#define DHT_TYPE DHT22  // DHT sensor type (DHT11, DHT21, or DHT22)

// BLE configuration
const char* localName = "SensorDHT22"; 
const char* serviceUuid = "19B10000-E8F2-537E-4F6C-D104768A1214";
const char* characteristicUuid = "19B10001-E8F2-537E-4F6C-D104768A1214";

// BLE service and characteristic setup
BLEService sensorService(serviceUuid);                               // Define service with UUID
BLEIntCharacteristic tempCharacteristic(characteristicUuid, BLERead); // Define characteristic with UUID, read permission

DHT dht = DHT(DATA_PIN, DHT_TYPE);   // Initialize DHT sensor

void setup() {
    dht.begin();                				

    if (!BLE.begin()) {                 		// Initialize BLE
        while (1);
    }

    // Setup BLE peripheral
    BLE.setLocalName(localName);                       // Set local name
    BLE.setAdvertisedService(sensorService);           // Assign service with UUID
    sensorService.addCharacteristic(tempCharacteristic); // Add characteristic
    BLE.addService(sensorService);                      // Add service to peripheral
    tempCharacteristic.writeValue(0);    		          // Set initial characteristic value
    BLE.advertise();                        		      // Start broadcasting service
}

unsigned long previousMillis = 0;                // Variable to control last reading

void loop() {
    BLEDevice central = BLE.central();      // Check if any device is connected

    if (central) {                          // If device is connected
        while (central.connected()) {         // While connection exists
            long currentMillis = millis();     // Update current time
            
            if (currentMillis - previousMillis >= 2000) { // If two seconds have passed since last reading
                float temperature = dht.readTemperature();       // Read sensor
                int intTemperature = (int)temperature;           // Convert value to integer
                tempCharacteristic.writeValue(intTemperature);   // Write value to characteristic
                
                previousMillis = currentMillis;             // Update reading time
            }
        }
    }
}
