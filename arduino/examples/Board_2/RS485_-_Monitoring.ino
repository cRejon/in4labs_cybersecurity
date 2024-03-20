#include <SoftwareSerial.h>
#include <DHT.h>  					//inclusión de la libreria para el sensor de temperatura

//SoftwareSerial pins
#define RX_PIN 7                  // connected to RO (Receiver Out)
#define TX_PIN 8                  // connected to DI (Driver In)
#define TRANSMIT_ENABLER 3        // high to send data. low to receive

#define RED_PIN 5  
#define GREEN_PIN 6
#define BLUE_PIN 9

#define DATA_PIN 2                   // define the type data pin
#define DHT_TYPE DHT22               // define the DHT sensor (DHT11, DHT21, or DHT22)

SoftwareSerial RS485 (RX_PIN, TX_PIN);     // assign name, RX and TX pins to software serial

DHT dht = DHT(DATA_PIN, DHT_TYPE);   // instantiate the dht class with our data pin and DHT type.


void setup() {
	RS485.begin(9600);          	//configuración de la frecuencia del puerto serie
	while (!RS485) {
		; 
	}

	dht.begin();                

  pinMode(GREEN_PIN, OUTPUT);    	// Led verde encendido significa que el puerto serie esta libre
  pinMode(BLUE_PIN, OUTPUT);     	// Led azul no se usará de momento
  pinMode(RED_PIN, OUTPUT);      	// Led rojo encendido significa que el puerto serie esta ocupado
  analogWrite(GREEN_PIN, 255);    	// encender led verde
  analogWrite(BLUE_PIN, 0);   	// apagar led azul  
  analogWrite(RED_PIN, 0);		// apagar led rojo

  pinMode(TRANSMIT_ENABLER, OUTPUT);      // output signal to control MAX485 mode
  digitalWrite(TRANSMIT_ENABLER, LOW);    // setup as slave. MAX485 receiving data
}

long previousMillis = 0;    // variable para controlar la ultima vez que se ha leido el sensor
float temperatureRead;     // variable para guardar la lectura del sensor

void loop() {
	long currentMillis = millis();              
	if(currentMillis-previousMillis>= 2000){    	// Si la temperatura se ha leido hace mas de 2 seg
		temperatureRead = dht.readTemperature();  	// leer temperatura
		previousMillis=currentMillis;             	
		digitalWrite(TRANSMIT_ENABLER, HIGH); 		// habilitar el puerto serie para transmisión
		digitalWrite(GREEN_PIN, 0);   		// apagar led verde
		digitalWrite(RED_PIN, 255);       		// encender led rojo
		delay(10);
		RS485.write(temperatureRead.toInt());
		RS485.flush();                           
		digitalWrite(GREEN_PIN, 255);    		// encender el led verde
		digitalWrite(RED_PIN, 0);   			// apagar el led rojo 
		digitalWrite(TRANSMIT_ENABLER,LOW); 			// deshabilita el puerto serie para transmisión
	}
}
