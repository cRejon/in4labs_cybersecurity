#include <ArduinoBLE.h> //Incluir libreria BLE
#include <DHT.h>    	//Incluir libreria del sensor de temperatura

#define RED_PIN A0  
#define GREEN_PIN A1
#define BLUE_PIN A2

#define DATA_PIN 2                   // define the type data pin
#define DHT_TYPE DHT22               // define the DHT sensor (DHT11, DHT21, or DHT22)

DHT dht = DHT(DATA_PIN, DHT_TYPE);   // instantiate the dht class with our data pin and DHT type

BLEService sensorService("0005");			// Define el servicio con UUID
BLECharacteristic temperatureChar("2A21", BLERead | BLENotify,5); // Define la caracteristica con UUID, lectura y notificaciones

void setup() {
  dht.begin();                				

  if (!BLE.begin()) {                 		// Inicializar BLE
    while (1);
  }

  BLE.setLocalName("DHT22SensorTEST");               // Establezco nombre local
  BLE.setAdvertisedService(sensorService);          // Asigno servicio con UUID
  sensorService.addCharacteristic(temperatureChar); // Añado caracteristica
  BLE.addService(sensorService);                    // Añado servicio al periférico
  temperatureChar.writeValue("10.00");    			// Establezco un valor inicial a la caracteristica
  BLE.advertise();                        			// Se empieza a difundir el servicio

  pinMode(GREEN_PIN, OUTPUT);      // Se define el pin del led verde como salida
  pinMode(BLUE_PIN, OUTPUT);       // Se define el pin del led azul como salida
  pinMode(RED_PIN, OUTPUT);        // Se define el pin del led rojo como salida
  analogWrite(GREEN_PIN, 255);      // Se enciende el led verde
  analogWrite(BLUE_PIN, 0);     // Se apaga el led azul
  analogWrite(RED_PIN, 0);		// Se apaga el led rojo
}


void loop() {
  String sensorRead;                      // Variable para almacenar la temperatura
  long previousMillis = 0;                // Variable para controlar ultima lectura
  
  BLEDevice central = BLE.central();      // Verifica si hay algun dispositivo conectado

  if (central) {                          // Si hay un dispositivo conectado
    analogWrite(GREEN_PIN, 0);    // Apago el led verde
    analogWrite(BLUE_PIN, 255);        // Enciendo el led azul

    while (central.connected()) {         // Mientra exista conexión
      long currentMillis = millis();      // Actualizo el valor tiempo actual
      
      if (currentMillis - previousMillis >= 2000) { // Si han pasado dos segundos desde la ultima lectura tiempo_actual-ultima_lectura
        float temperature = dht.readTemperature();         // Leo el sensor
        sensorRead = String(temperature, 1);               // Paso el valor a String con un decimal
        char tempChar[6];                           // Variable cadena de caracteres para enviar por BLE
        sensorRead.toCharArray(tempChar,6);         // Paso el String a tabla de caracteres
        temperatureChar.writeValue(tempChar,5);     // Envio la tabla de caracteres, especificando que son 5
        
        previousMillis = currentMillis;             // Actualizo el valor del tiempo de lectura
      }
    }
  // close the connection:
  analogWrite(GREEN_PIN, 255);    // Enciendo el led verde
  analogWrite(BLUE_PIN, 0);   // Apago el led azul
  }
}
