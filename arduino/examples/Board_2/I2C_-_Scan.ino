#include <Wire.h> 			// incluye la libreria Wire
#include <DHT.h>  			// incluye la libreria DHT

#define RED_PIN 5  
#define GREEN_PIN 6
#define BLUE_PIN 9

#define DATA_PIN 2                   // define the type data pin
#define DHT_TYPE DHT22               // define the DHT sensor (DHT11, DHT21, or DHT22)

DHT dht = DHT(DATA_PIN, DHT_TYPE);   // instantiate the dht class with our data pin and DHT type.

void setup() {
  dht.begin(); 

  Wire.begin(9);					// estableciendo dirección 9
  Wire.setClock(10000);				// establezco la velocidad del bus en lenta
  Wire.onReceive(receiveEvent);		// si se recibe algún dato pasarlo a la funcion 'receiveEvent'
  Wire.onRequest(requestEvent); 	// si se requiere algun dato ejecutar la funcion 'requestEvent'

  pinMode(GREEN_PIN, OUTPUT);	// Led verde encendido significa que el puerto serie esta libre
  pinMode(BLUE_PIN, OUTPUT);   // Led azul no se usará de momento
  pinMode(RED_PIN, OUTPUT);    // Led rojo encendido significa que el puerto serie esta ocupado
  digitalWrite(GREEN_PIN, 255);  // encender led verde
  digitalWrite(BLUE_PIN, 0); // apagar led azul  
  digitalWrite(RED_PIN, 0);	// apagar led rojo
}

String temperatureRead;				// variable para guardar la lectura del sensor

void loop() {
  delay(2000);            		//Espera activa de 2 segundo
}

void requestEvent(){
      Wire.write(temperatureRead.toInt());  // se envia un byte con el valor de la temperatura
}

void receiveEvent(int numBytes){
  digitalWrite(GREEN_PIN, 0);  	// apagar led verde
  digitalWrite(RED_PIN,255);       	// encender led rojo
  if (Wire.available()){				// hay datos que leer
	if (Wire.read()=='T'){            	 
		float temperature = dht.readTemperature(); 	
        temperatureRead = String(temperature, 1);  // convertir el valor de la temperatura a String
	}
  }
  digitalWrite(GREEN_PIN, 255);    	// encender el led verde
  digitalWrite(RED_PIN, 0);   	// apagar el led rojo	
}