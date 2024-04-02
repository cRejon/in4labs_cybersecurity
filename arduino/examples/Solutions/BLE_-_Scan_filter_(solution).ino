#include <ArduinoBLE.h> //Incluir libreria BLE
#include <DHT.h>  			// incluye la libreria DHT

#define RED_PIN 5  
#define GREEN_PIN 6
#define BLUE_PIN 9

#define DATA_PIN 2                   // define the type data pin
#define DHT_TYPE DHT22               // define the DHT sensor (DHT11, DHT21, or DHT22)

DHT dht = DHT(DATA_PIN, DHT_TYPE);   // instantiate the dht class with our data pin and DHT type.                            

BLEService sensorService("0005"); 	// Define el servicio con UUID

BLECharacteristic temperatureChar("2A21", BLERead | BLENotify,5); // Define la caracteristica con UUID, lectura y notificaciones

void setup() {
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

  pinMode(GREEN_PIN, OUTPUT);    	// Led verde encendido significa que la placa de enciende
  pinMode(BLUE_PIN, OUTPUT);     	// Led azul encendido significará que esta conectado
  pinMode(RED_PIN, OUTPUT);      	// Led rojo encendido significa que esta verificando una conexión
  digitalWrite(GREEN_PIN, 255);    	// encender led verde
  digitalWrite(BLUE_PIN, 0);   	// apagar led azul  
  digitalWrite(RED_PIN, 0);		// apagar led rojo
}


void loop() {
  String sensorRead;                      // String para almacenar la temperatura
  long previousMillis = 0;                // En milisegundo, valor del ultimo muestreo
  
  BLEDevice central = BLE.central();      // Esperar a que la central se conecte

  if (central) {                          // Si se ha conectado un maestro
    digitalWrite(GREEN_PIN, 0);    // Apagar led verde
    digitalWrite(BLUE_PIN, 255);        // Encender led azul

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
  digitalWrite(GREEN_PIN, 255);    // Encender led verde
  digitalWrite(BLUE_PIN, 0);   // Apagar led azul 
  }
}

void blePeripheralConnectHandler(BLEDevice central) {
  digitalWrite(GREEN_PIN, 0);      		// Apagar led verde
  digitalWrite(RED_PIN, 255);		  		// Encendemos el led rojo para indicar que hay un intento de conexión
  id (central.address()!="40:11:ae:d5:80:82") {	//Verificamos que la MAC sea la de la placa AR3
	central.disconnect();						// si la MAC es distinta desconectamos el dispositivo
  }
  digitalWrite(GREEN_PIN, 255);      		// Encendemos el led verde
  digitalWrite(RED_PIN, 0);				// Apagamos el led rojo
}