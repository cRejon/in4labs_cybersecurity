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
  analogWrite(GREEN_PIN, 0);	// Apaga el LED VERDE
  analogWrite(BLUE_PIN, 255);   	// Enciende el led azul para indicar que esta buscando UUID  
  analogWrite(RED_PIN, 0);		// Apaga el LED ROJO
  BLE.scanForUuid("0005");     			// Escaneo en busca del dispositivo con uuid 
  peripheral = BLE.available(); 

  int counter = 0;             			// Establezco el contador a 0
  
  while(!peripheral && counter < 100){  // Mientras no lo encuentre y el contador sea menor de 100
    BLE.scanForUuid("0005");            // Escaneo en busca del dispositivo con uuid 
    delay(10);                                   
    peripheral = BLE.available();       // Sera verdadero si hay dispositivo con ese UUID        
    counter =counter+1;                 // incrementar contador
  }  
  analogWrite(GREEN_PIN, 255);    	// Enciende el LED VERDE
  analogWrite(BLUE_PIN, 0);   	// Apaga el LED AZUL- ha parado la busqueda  
  analogWrite(RED_PIN, 0);		// Apaga el LED ROJO
  if (peripheral) {						// Si ha encontrado un periférico
	if (peripheral.connect()) {  		// Se conecta al BLE
		analogWrite(GREEN_PIN, 0);  // apago  el led verde
		analogWrite(BLUE_PIN, 0);    // apago el led azul
		analogWrite(RED_PIN, 255);		//enciendo el led rojo ya estoy conectado al dispositivo
		while(1);							// bucle infinito para bloquear la conexión con el dispositivo					
		delay(10);                          // small delay to smooth communication
    }
  }
  analogWrite(GREEN_PIN, 255);    	// Enciende el LED VERDE
  analogWrite(BLUE_PIN, 0);   	// Apaga el LED AZUL- ha parado la busqueda  
  analogWrite(RED_PIN, 0);		// Apaga el LED ROJO
}
