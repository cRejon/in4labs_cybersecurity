#include <Wire.h> 		

#define FAN_PIN 2             	// Pin de conexión del transistor que activa el ventilador
#define TEMP_LIMIT 25         	// Límite de temperatura

#define RED_PIN 5  
#define GREEN_PIN 6
#define BLUE_PIN 9

void setup() {
  Wire.begin(10);                    	// estableciendo dirección 10
  Wire.setClock(10000);					// establezco la velocidad del bus en lenta

  pinMode(GREEN_PIN, OUTPUT);    	// Led verde encendido significa que el puerto serie esta libre
  pinMode(BLUE_PIN, OUTPUT);     	// Led azul no se usará de momento
  pinMode(RED_PIN, OUTPUT);      	// Led rojo encendido significa que el puerto serie esta ocupado
  digitalWrite(GREEN_PIN, 0);  	// encender led verde
  digitalWrite(BLUE_PIN, 255);   	// apagar led azul  
  digitalWrite(RED_PIN, 255);	// apagar led rojo

  digitalWrite(FAN_PIN, HIGH);            	// Apagar ventilador
}

int temperature;            // var almacena temperatura
long previousMillis = 0;

void loop() {
  temperature= 0;       
  long currentMillis = millis();              
  if(currentMillis-previousMillis>= 20000){    	// Si la temperatura se ha leido hace mas de 20seg
	previousMillis=currentMillis;
	digitalWrite(GREEN_PIN, 0);  	// apagar led verde
	digitalWrite(RED_PIN, 255);       	// encender led rojo	
	Wire.beginTransmission (9);		// empezar transmisión con el esclavo 9
    Wire.write("T");				// se envia un byte concretamente la letra T
    Wire.endTransmission ();		// se termina la transmisión
    delay (100);					// espera para que se actualice el valor de temp
	Wire.requestFrom(9,1);			// solicito del esclavo 9 el envío de un byte
    if (Wire.available()){			// si el dato esta disponible en el BUS
		char c=Wire.read();			// leo un byte
		temperature=int(c);
		if (temperature>TEMP_LIMIT)             // si la temperatura es mayor que el límite
			digitalWrite(FAN_PIN, LOW);   // encender el ventilador   
		else
			digitalWrite(FAN_PIN, HIGH);   // apagar el ventilador
    }
	digitalWrite(GREEN_PIN, 255);	// encender el led verde
	digitalWrite(RED_PIN, 0);   	// apagar el led rojo
  }
}