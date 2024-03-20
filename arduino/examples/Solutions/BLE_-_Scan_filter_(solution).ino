#include <ArduinoBLE.h> //Incluir libreria BLE
#include <Wire.h>   	//Icluir libreria para la pantalla
#include <DHT.h>    	//Incluir libreria del sensor de temperatura

#define dataPin 10                  // Define pin del sensor
#define DHTType DHT22               // Define tipo de sensor

DHT dht = DHT(dataPin, DHTType);    // Crea objeto del sensor con pin y tipo

#define redStatusPin 5        		// Define pin led rojo
#define greenStatusPin 6      		// Define pin led verde
#define blueStatusPin 9       		// Define pin led azul
                              

/
BLEService sensorService("0005"); 	// Define el servicio con UUID

BLECharacteristic temperatureChar("2A21", BLERead | BLENotify,5); // Define la caracteristica con UUID, lectura y notificaciones

void setup() {
  Wire.begin(2);               

  pinMode(controlSPIBusClock, OUTPUT);          
  digitalWrite(controlSPIBusClock, LOW);        

  dht.begin();                // Inicializar Sensor de temperatura

  if (!BLE.begin()) {         // Inicializar BLE
    while (1);
  }

  BLE.setLocalName("DH22SensorTEST");               // Establecer nombre local del servicio
  BLE.setAdvertisedService(sensorService);          // Establecer identificado del servicio
  sensorService.addCharacteristic(temperatureChar); // Añadir caracteristica al servicio
  BLE.addService(sensorService);                    // Añadir el servicio al periférico
  temperatureChar.writeValue("10.00");    			// Establacer un valor inicial de temperatura
  
  BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);  //se crea un evento para controlar quien conecta
  
  BLE.advertise();                        			

  pinMode(greenStatusPin, OUTPUT);    	// Led verde encendido significa que la placa de enciende
  pinMode(blueStatusPin, OUTPUT);     	// Led azul encendido significará que esta conectado
  pinMode(redStatusPin, OUTPUT);      	// Led rojo encendido significa que esta verificando una conexión
  digitalWrite(greenStatusPin, 0);    	// encender led verde
  digitalWrite(blueStatusPin, 255);   	// apagar led azul  
  digitalWrite(redStatusPin, 255);		// apagar led rojo
}


void loop() {
  String sensorRead;                      // String para almacenar la temperatura
  long previousMillis = 0;                // En milisegundo, valor del ultimo muestreo
  
  BLEDevice central = BLE.central();      // Esperar a que la central se conecte

  if (central) {                          // Si se ha conectado un maestro
    digitalWrite(greenStatusPin, 255);    // Apagar led verde
    digitalWrite(blueStatusPin,0);        // Encender led azul

    while (central.connected()) {         
      long currentMillis = millis();      
      
      if (currentMillis - previousMillis >= 2000) { // Si han pasado dos segundos desde la ultima lectura
        sensorRead = dht.readTemperature();         // Leer temperatura
        char tempChar[6];                           
        sensorRead.toCharArray(tempChar,6);         // Pasar la lectura del sensor a un array de caracteres
        temperatureChar.writeValue(tempChar,5);     // Enviar el array a la central
        
        previousMillis = currentMillis;            
      }
    }
  // close the connection:
  digitalWrite(greenStatusPin, 0);    // Encender led verde
  digitalWrite(blueStatusPin, 256);   // Apagar led azul 
  }
}

void blePeripheralConnectHandler(BLEDevice central) {
  digitalWrite(greenStatusPin, 255);      		// Apagar led verde
  digitalWrite(redStatusPin, 0);		  		// Encendemos el led rojo para indicar que hay un intento de conexión
  id (central.address()!="40:11:ae:d5:80:82") {	//Verificamos que la MAC sea la de la placa AR3
	central.disconnect();						// si la MAC es distinta desconectamos el dispositivo
  }
  digitalWrite(greenStatusPin, 0);      		// Encendemos el led verde
  digitalWrite(redStatusPin, 255);				// Apagamos el led rojo
}