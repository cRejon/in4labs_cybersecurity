#include <ArduinoBLE.h>


const char* peripheralName = "SensorDHT22"; 

void setup() {
    Serial.begin(9600);
    while (!Serial) {
        ;                         // Serial used for communication with other boards
    }
    
    if (!BLE.begin()) {
        while (1);
    }

    // start scanning for peripherals
    BLE.scan();
}

void loop() {
    // check if a peripheral has been discovered
    BLEDevice peripheral = BLE.available();

    if (peripheral) {
        // your code here





        // see if peripheral is our desired one
        if (your code here) {
            // stop scanning
            BLE.stopScan();
            // explore services of the peripheral
            exploreServices(peripheral);

            // we are done, disconnect
            Serial.println("Disconnecting ...");
            peripheral.disconnect();
            while (1) {
                //  nothing to do
            }
        }
    }

    delay(1000);  
}

void exploreServices(BLEDevice peripheral) {
    BLEService service;

    // try to connect to the peripheral
    if (peripheral.connect()) {
        // your code here




        // loop the services of the peripheral, print UUID of each and explore its characteristics
        for (int i = 0; i < peripheral.serviceCount(); i++) {
            // your code here



            exploreCharacteristics(service);
        }
    } else {
        Serial.println("Failed to connect!");
    }
}


void exploreCharacteristics(BLEService service) {
    BLECharacteristic characteristic;

    // your code here



}