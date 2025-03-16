#include <ArduinoBLE.h> // Include BLE library

// Pin and threshold configuration
#define FAN_PIN 2        // Fan transistor connection pin
#define TEMP_LIMIT 30    // Temperature threshold
#define DELAY 20000      // Wait time

// BLE configuration
const char* serviceUuid = "19B10000-E8F2-537E-4F6C-D104768A1214";
const char* characteristicUuid = "19B10001-E8F2-537E-4F6C-D104768A1214";

// Global variables
int temp = 0;
BLEDevice peripheral;                    // Reference to peripheral device
BLECharacteristic peripheralCharacteristic; // Reference to characteristic

void setup() {
    pinMode(FAN_PIN, OUTPUT);         // Set fan pin as output
    digitalWrite(FAN_PIN, LOW);       // Turn off fan

    BLE.begin();                      // Initialize BLE
}

void loop() {    
    do {
        BLE.scanForUuid(serviceUuid);  // use serviceUuid variable
        peripheral = BLE.available();
        delay(100);
    } while (!peripheral);  

    BLE.stopScan();               // Stop scanning

    if (peripheral.connect()) {  // connecting to peripheral
        delay(10);                          // small delay to smooth communication
                        
        if (peripheral.discoverService(serviceUuid)) {  // use serviceUuid variable
            peripheralCharacteristic = peripheral.characteristic(characteristicUuid);  // use characteristicUuid variable
            peripheralCharacteristic.readValue(temp); 		 // Read characteristic value
        }
        
        if (temp > TEMP_LIMIT){	            // if temperature exceeds limit
            digitalWrite(FAN_PIN, HIGH);    // Turn on fan    
        } else {
            digitalWrite(FAN_PIN, LOW);     // Turn off fan
        }

        peripheral.disconnect();            // Disconnect peripheral       
    }
  
    delay(DELAY);                     
}
