#include <Wire.h> 		

#define RED_PIN 5  
#define GREEN_PIN 6
#define BLUE_PIN 9

void setup() {
  Wire.begin(9);                    	// estableciendo dirección 9
  Wire.setClock(10000);			// establezco la velocidad del bus en lenta
  Wire.onReceive(receiveEvent);		// si se recibe algún dato pasarlo a la funcion 'receiveEvent'
  Wire.onRequest(receptionEvent); 	// si se requiere algun dato ejecutar la funcion 'receptionEvent'
  pinMode(GREEN_PIN, OUTPUT);    	// Led verde encendido significa que el puerto serie esta libre
  pinMode(BLUE_PIN, OUTPUT);     	// Led azul no se usará de momento
  pinMode(RED_PIN, OUTPUT);      	// Led rojo encendido significa que el puerto serie esta ocupado
  digitalWrite(GREEN_PIN, 255);  	// encender led verde
  digitalWrite(BLUE_PIN, 0);   	// apagar led azul  
  digitalWrite(RED_PIN, 0);	// apagar led rojo
}

long previousMillis = 0;
String temperatureRead;	

void loop() {
   delay(1000);            		//Espera activa de 1 segundo
}

void receptionEvent(){
      Wire.write(temperatureRead.toInt());  // se envia el valor 100
}

void receiveEvent(int numBytes){
  digitalWrite(GREEN_PIN, 0);  	// apagar led verde
  digitalWrite(RED_PIN, 255);       	// encender led rojo
  if (Wire.available()){				// hay datos que leer
	if (Wire.read()=='T'){            	 
		temperatureRead = "100"; 	
	}
  }
  digitalWrite(GREEN_PIN, 255);    	// encender el led verde
  digitalWrite(RED_PIN, 0);   	// apagar el led rojo	
}