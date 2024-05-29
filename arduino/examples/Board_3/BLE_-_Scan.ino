
#include <ArduinoBLE.h>		//Incluir libreria BLE
#include <Wire.h>   		//Icluir libreria para la pantalla

#define FAN_PIN 2             	// Pin de conexión del transistor que activa el ventilador
# define TEMP_LIMIT 25         	// Límite de temperatura

#define RED_PIN A0  
#define GREEN_PIN A1
#define BLUE_PIN A2

void setup() {
  BLE.begin();                      // Se inicializa BLE

  pinMode(GREEN_PIN, OUTPUT);      // Se define el pin del led verde como salida
  pinMode(BLUE_PIN, OUTPUT);       // Se define el pin del led azul como salida
  pinMode(RED_PIN, OUTPUT);        // Se define el pin del led rojo como salida
  analogWrite(GREEN_PIN, 255);      // Se enciende el led verde
  analogWrite(BLUE_PIN, 0);     // Se apaga el led azul
  analogWrite(RED_PIN, 0);		// Se apaga el led rojo

  digitalWrite(FAN_PIN, HIGH);      		// Se define el pin del ventilador a 0--Apagado
}

String temperatureRead;         // Variable para almacenar la temperatura
float temperatureFloat;         // Variable para almacenar la temperatura y poder compararla con el limite
char peripheralChar [5];        // Tabla de caracteres recibida desde BLE
BLEDevice peripheral;           		    // Variable para referenciar al periférico
BLECharacteristic peripheralCharacteristic; // Variable para referenciar a la caracteristica

void loop() {                   
  temperatureRead = "";         
  BLE.scanForUuid("0005");      // Escaneo en busca del servicio con UUID
  peripheral = BLE.available(); // Si esta disponible lo instancio en peripherial

  int counter = 0;              // Establezco el contador a 0
  
  while(!peripheral && counter < 100){  // Mientras no este conectado y no llegue a 100 el contador
    BLE.scanForUuid("0005");            // Escaneo en busca del servicio con UUID
    delay(10);                          
    peripheral = BLE.available();      	// Si esta disponible lo instancio en peripherial   
    counter =counter+1;                 
  }  
  BLE.stopScan();               // Parar el escaneo

  if (!peripheral.connect()) {  // Si no estoy conectado
                                // No hago nada
  } else {                      // Si esta conectado

	analogWrite(GREEN_PIN, 0);  // Apago el led verder
	analogWrite(BLUE_PIN, 255);      // Enciendo el led azul
	delay(10);                          
  
    if (!peripheral.discoverService("0005")) {  // Si no estoy conectado con el servicio
                                                // No hago nada
    } else {                                    // Si estoy conectado con el servicio

      peripheralCharacteristic = peripheral.characteristic("2A21"); //Conecto con la caracteristica
      
      peripheralCharacteristic.readValue(peripheralChar,5); 		// Leo el valor de la caracteristica
      for (int i = 0; i < 5; i++){                          		// La transferencia en byte a byte
        temperatureRead += peripheralChar[i]; 
      }
    }
    
    peripheral.disconnect();            // Desconecto el periférico       
    analogWrite(GREEN_PIN, 255);    // Enciendo el led verde
    analogWrite(BLUE_PIN, 0);   // Apago el led azul
   
  }
  
  temperatureFloat = temperatureRead.toFloat(); // Convierto la temperatura
  
  if ( temperatureFloat > temperatureLimit){	//si supera el limite
    digitalWrite(FAN_PIN, HIGH);   				// Enciendo el ventilador    
  } else {
    digitalWrite(FAN_PIN, LOW);             		// Apago el ventilador
  }
  
  delay(20000);                     
  
}
