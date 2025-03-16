#include <ArduinoBLE.h> // Include BLE library

// Pin and threshold configuration
#define FAN_PIN 2        // Fan transistor connection pin
#define TEMP_LIMIT 30    // Temperature threshold
#define DELAY 20000      // Wait time

// BLE configuration
const char* peripheralAddress = "00:00:00:00:00:00"; // MAC of the peripheral device
const char* serviceUuid = "19B10000-E8F2-537E-4F6C-D104768A1214";
const char* characteristicUuid = "19B10001-E8F2-537E-4F6C-D104768A1214";

// Global variables
int temp = 0;
BLEDevice peripheral;                    // Reference to peripheral device
BLECharacteristic peripheralCharacteristic; // Reference to characteristic

void blePeripheralConnectHandler(BLEDevice peripheral) {
    if (peripheral.address() != peripheralAddress) {	
        peripheral.disconnect();	
    }
}

void setup() {
    pinMode(FAN_PIN, OUTPUT);         // Set fan pin as output
    digitalWrite(FAN_PIN, LOW);       // Turn off fan

    if (!BLE.begin()) {              // Initialize BLE
        while (1);
    }

    BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
}

void loop() {    
    do {
        BLE.scanForUuid(serviceUuid);  // Use serviceUuid variable
        peripheral = BLE.available();
        delay(100);
    } while (!peripheral);  

    BLE.stopScan();               // Stop scanning

    if (peripheral.connect()) {  // Connecting to peripheral
        delay(10);              // Small delay to smooth communication
                        
        if (peripheral.discoverService(serviceUuid)) {
            peripheralCharacteristic = peripheral.characteristic(characteristicUuid);
            peripheralCharacteristic.readValue(temp); 		// Read characteristic value
        }

        if (temp > TEMP_LIMIT){	        // If temperature exceeds limit
            digitalWrite(FAN_PIN, HIGH);  // Turn on fan    
        } else {
            digitalWrite(FAN_PIN, LOW);   // Turn off fan
        }

        peripheral.disconnect();          // Disconnect peripheral       
    }
  
    delay(DELAY);                     
}
