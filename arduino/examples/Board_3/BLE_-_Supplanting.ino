#include <ArduinoBLE.h>		//Incluir libreria BLE

#define FAN_PIN 2             	// Pin de conexión del transistor que activa el ventilador

#define RED_PIN A0  
#define GREEN_PIN A1
#define BLUE_PIN A2
 

void setup() {
  BLE.begin();                 // Se inicializa BLE

  pinMode(GREEN_PIN, OUTPUT);      // Se define el pin del led verde como salida
  pinMode(BLUE_PIN, OUTPUT);       // Se define el pin del led azul como salida
  pinMode(RED_PIN, OUTPUT);        // Se define el pin del led rojo como salida
  analogWrite(GREEN_PIN, 255);      // Se enciende el led verde
  analogWrite(BLUE_PIN, 0);     // Se apaga el led azul
  analogWrite(RED_PIN, 0);		// Se apaga el led rojo

  digitalWrite(FAN_PIN, LOW);      		// Se apaga el ventilador
}

BLEDevice peripheral;                       // Variable para referenciar al periférico
BLECharacteristic peripheralCharacteristic; // Variable para referenciar a la caracteristica

void loop() {
  
}
