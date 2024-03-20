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

void loop() {
	if (RS485.available()){
		delay(200);
		if (RS485.read()=='I') {            		// espera a que haya en el BUS un inicio de trama
			if (RS485.read()=='T'){
				if (RS485.read()=='F'){
					analogWrite(GREEN_PIN, 0);  // apagar el led verde
					analogWrite(RED_PIN, 255);      	// encender el led rojo
					digitalWrite(TRANSMIT_ENABLER, HIGH); // habilita el puerto para transmision
					RS485.write("I");
					RS485.write(100);
					RS485.write("F");
					RS485.flush();
					digitalWrite(TRANSMIT_ENABLER, LOW);  // deshabilita el puerto serie para transmisión
					delay(100);
					analogWrite(GREEN_PIN, 255);    // encender el led verde
					analogWrite(RED_PIN, 0);    // apagar el led rojo 
					digitalWrite(TRANSMIT_ENABLER, LOW);  // deshabilita el puerto serie para transmisión
				}
				else {
					clear_bus();
				}
			}
			else {
				clear_bus();
			}
		}
		else {
			clear_bus();
		}
	}
}

void clear_bus() {
	while (RS485.available()>0) {
		RS485.read();
	}
}
