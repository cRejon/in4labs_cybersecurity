#include <Wire.h> 		

#define RED_PIN A0  
#define GREEN_PIN A1
#define BLUE_PIN A2

void setup() {
  Wire.begin(9);                    	// estableciendo dirección 9
  Wire.setClock(10000);			// establezco la velocidad del bus en lenta
  Wire.onReceive(receiveEvent);		// si se recibe algún dato pasarlo a la funcion 'receiveEvent'
  Wire.onRequest(receptionEvent); 	// si se requiere algun dato ejecutar la funcion 'receptionEvent'
  
  pinMode(GREEN_PIN, OUTPUT);    	// Led verde encendido significa que el puerto serie esta libre
  pinMode(BLUE_PIN, OUTPUT);     	// Led azul no se usará de momento
  pinMode(RED_PIN, OUTPUT);      	// Led rojo encendido significa que el puerto serie esta ocupado
  analogWrite(GREEN_PIN, 255);  	// encender led verde
  analogWrite(BLUE_PIN, 0);   	// apagar led azul  
  analogWrite(RED_PIN, 0);	// apagar led rojo
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
  analogWrite(GREEN_PIN, 0);  	// apagar led verde
  analogWrite(RED_PIN, 255);       	// encender led rojo
  if (Wire.available()){				// hay datos que leer
	if (Wire.read()=='T'){            	 
		temperatureRead = "100"; 	
	}
  }
  analogWrite(GREEN_PIN, 255);    	// encender el led verde
  analogWrite(RED_PIN, 0);   	// apagar el led rojo	
}