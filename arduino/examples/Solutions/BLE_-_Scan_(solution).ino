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
        Serial.println("Discovered a peripheral");

        // print out address 
        Serial.print("Address: ");
        Serial.println(peripheral.address());
        // print the local name, if present
        if (peripheral.hasLocalName()) {
            Serial.print("Local Name: ");
            Serial.println(peripheral.localName());
        }

        // see if peripheral is our desired one
        if (peripheral.localName() == peripheralName) {
            // stop scanning
            BLE.stopScan();
            // explore services of the peripheral
            exploreServices(peripheral);

            // we are done, disconnect
            Serial.println("Disconnecting ...");
            peripheral.disconnect();
            while (1) {
                // do nothing
            }
        }
    }

    delay(1000);  
}

void exploreServices(BLEDevice peripheral) {
    BLEService service;

    // try to connect to the peripheral
    if (peripheral.connect()) {
        Serial.println("Connected");

        // discover peripheral attributes
        Serial.println("Discovering attributes ...");
        if (peripheral.discoverAttributes()) {
            Serial.println("Attributes discovered");
        } else {
            Serial.println("Attribute discovery failed!");
            peripheral.disconnect();
            return;
        }

        // loop the services of the peripheral, print UUID of each and explore its characteristics
        for (int i = 0; i < peripheral.serviceCount(); i++) {
            service = peripheral.service(i);
            Serial.print("Service ");
            Serial.println(service.uuid());

            exploreCharacteristics(service);
        }
    } else {
        Serial.println("Failed to connect!");
    }
}


void exploreCharacteristics(BLEService service) {
    BLECharacteristic characteristic;

    // loop the characteristics of the service and explore each
    for (int i = 0; i < service.characteristicCount(); i++) {
        characteristic = service.characteristic(i);
        // print the UUID and properties of the characteristic
        Serial.print("\tCharacteristic ");
        Serial.print(characteristic.uuid());

        // check if the characteristic is readable
        if (characteristic.canRead()) {
            // read the characteristic value
            int value;
            characteristic.readValue(value);
            Serial.print(", value ");
            Serial.print(value);
        }
        Serial.println();
    }
}