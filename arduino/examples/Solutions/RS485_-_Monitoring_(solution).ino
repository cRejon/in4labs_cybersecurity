#include <SoftwareSerial.h>

//SoftwareSerial pins
#define RX_PIN 7                  // connected to RO (Receiver Out)
#define TX_PIN 8                  // connected to DI (Driver In)
#define TRANSMIT_ENABLER 3        // high to send data. low to receive

#define RED_PIN 5  
#define GREEN_PIN 6
#define BLUE_PIN 9

SoftwareSerial RS485 (RX_PIN, TX_PIN);     // assign name, RX and TX pins to software serial

void setup() {
	RS485.begin(9600);  //configuración de la frecuencia del puerto serie
	while (!RS485) {
		; 
	}

  pinMode(GREEN_PIN, OUTPUT);    	// Led verde encendido significa que el puerto serie esta libre
  pinMode(BLUE_PIN, OUTPUT);     	// Led azul no se usará de momento
  pinMode(RED_PIN, OUTPUT);      	// Led rojo encendido significa que el puerto serie esta ocupado
  analogWrite(GREEN_PIN, 255);    	// encender led verde
  analogWrite(BLUE_PIN, 0);   	// apagar led azul  
  analogWrite(RED_PIN, 0);		// apagar led rojo

  pinMode(TRANSMIT_ENABLER, OUTPUT);      // output signal to control MAX485 mode
  digitalWrite(TRANSMIT_ENABLER, LOW);    // setup as slave. MAX485 receiving data
}

int temperature;     				// variable para almacenar la temperatura leida

void loop() {
	
	temperature = 0;     				// inicializar temperatura
	
	if(RS485.available()){              	// hay datos para leer en el puerto serie
		digitalWrite(GREEN_PIN, 0);  // apagar led verde
		digitalWrite(RED_PIN, 255);       // encender led rojo
		delay(200);
		temperature=int(RS485.read());
		delay(100);                           
		digitalWrite(GREEN_PIN, 255);    		// encender el led verde
		digitalWrite(RED_PIN, 0);   			// apagar el led rojo 
	}
}
