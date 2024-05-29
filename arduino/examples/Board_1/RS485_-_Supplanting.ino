#include <DHT.h>  					//inclusión de la libreria para el sensor de temperatura

#define RED_PIN A0  
#define GREEN_PIN A1
#define BLUE_PIN A2

//HardwareSerial pins
#define TX_PIN 7                  
#define RX_PIN 8  


#define DATA_PIN 2                   // define the type data pin
#define DHT_TYPE DHT22               // define the DHT sensor (DHT11, DHT21, or DHT22)

DHT dht = DHT(DATA_PIN, DHT_TYPE);   // instantiate the dht class with our data pin and DHT type.

void setup() {
    Serial1.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);  // initialize serial port
    delay(1000);          // wait for serial port to initialize   
	
    dht.begin();                

    pinMode(GREEN_PIN, OUTPUT);    	// Led verde encendido significa que el puerto serie esta libre
    pinMode(BLUE_PIN, OUTPUT);     	// Led azul no se usará de momento
    pinMode(RED_PIN, OUTPUT);      	// Led rojo encendido significa que el puerto serie esta ocupado
    analogWrite(GREEN_PIN, 255);    	// encender led verde
    analogWrite(BLUE_PIN, 0);   	// apagar led azul  
    analogWrite(RED_PIN, 0);		// apagar led rojo
}

String temperatureRead;     // variable para guardar la lectura del sensor

void loop() {
	             
	if (Serial1.available()){    									// hay datos en el BUS
		delay(100);
		if (Serial1.read()=='I'){									// detectamos el inicio de trama
			if (Serial1.read()=='T'){								// vemos si hay una solicitud de temperatura
				if (Serial1.read()=='F'){							// recibimos el fin por lo que trama es correcta
					temperatureRead = dht.readTemperature();  	// leer temperatura
					analogWrite(GREEN_PIN, 0);   		// apagar led verde
					analogWrite(RED_PIN, 255);       		// encender led rojo
					Serial1.write("I");
					Serial1.write(temperatureRead.toInt());
					Serial1.write("F");
					Serial1.flush();
					delay(100);
					analogWrite(GREEN_PIN, 0);    		// encender el led verde
					analogWrite(RED_PIN, 255);   			// apagar el led rojo 
				}
				else {
					Serial1.flush();
				}
			}
			else {
				Serial1.flush();
			}
		}
		else {
			Serial1.flush();
		}
		
	}
}
