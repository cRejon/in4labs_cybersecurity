#include <ArduinoBLE.h>		//Incluir libreria BLE

#define FAN_PIN 2             	// Pin de conexión del transistor que activa el ventilador
#define TEMP_LIMIT 30         	// Límite de temperatura
#define DELAY 20000           	// Tiempo de espera


const char* peripheralAddress = "00:00:00:00:00:00"; // MAC of the peripheral device

const char* serviceUuid = "19B10000-E8F2-537E-4F6C-D104768A1214";
const char* characteristicUuid = "19B10001-E8F2-537E-4F6C-D104768A1214";

int temp = 0;
BLEDevice peripheral;           		    // Variable para referenciar al periférico
BLECharacteristic peripheralCharacteristic; // Variable para referenciar a la caracteristica

void blePeripheralConnectHandler(BLEDevice peripheral) {
    // your code here

    
}

void setup() {
    pinMode(FAN_PIN, OUTPUT);         // Se define el pin del ventilador como salida
    digitalWrite(FAN_PIN, LOW);      		// Se apaga el ventilador

    if (!BLE.begin()) {                 		// Inicializar BLE
        while (1);
    }

    BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
}

void loop() {    
    do {
        BLE.scanForUuid(serviceUuid);  // use serviceUuid variable
        peripheral = BLE.available();
        delay(100);
    } while (!peripheral);  

    BLE.stopScan();               // Parar el escaneo

    if (peripheral.connect()) {  // connecting to peripheral
        delay(10);                          // small delay to smooth communication
                        
        if (peripheral.discoverService(serviceUuid)) {  // use serviceUuid variable
            peripheralCharacteristic = peripheral.characteristic(characteristicUuid);  // use characteristicUuid variable
            peripheralCharacteristic.readValue(temp); 		// Leo el valor de la caracteristica
        }

        if (temp > TEMP_LIMIT){	            //si supera el limite
            digitalWrite(FAN_PIN, HIGH);   				// Enciendo el ventilador    
        } else {
            digitalWrite(FAN_PIN, LOW);             	// Apago el ventilador
        }

        peripheral.disconnect();            // Desconecto el periférico       
    }
  
    delay(DELAY);                     
}
