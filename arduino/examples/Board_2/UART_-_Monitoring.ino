#include <DHT.h>                     // include the DHT library.

#define RED_PIN 5  
#define GREEN_PIN 6
#define BLUE_PIN 9

#define DATA_PIN 2                   // define the type data pin
#define DHT_TYPE DHT22               // define the DHT sensor (DHT11, DHT21, or DHT22)

DHT dht = DHT(DATA_PIN, DHT_TYPE);   // instantiate the dht class with our data pin and DHT type.

void setup() {
  Serial1.begin(115200);          	//configuración de la frecuencia del puerto serie
  while (!Serial1) {
	  ;
  }

  pinMode(GREEN_PIN, OUTPUT);    	// Led verde encendido significa que el puerto serie esta libre
  pinMode(BLUE_PIN, OUTPUT);     	// Led azul no se usará de momento
  pinMode(RED_PIN, OUTPUT);      	// Led rojo encendido significa que el puerto serie esta ocupado
  analogWrite(GREEN_PIN, 255);    	// encender led verde
  analogWrite(BLUE_PIN, 0);   	// apagar led azul  
  analogWrite(RED_PIN, 0);		// apagar led rojo

  dht.begin(); 
}

long previousMillis = 0;    // last time the sensor was checked, in ms
float temperature;     // make a float to hold data read from the sensor

void loop() {
  
  long currentMillis = millis();              // current time to be compared with previousMillis
  
  if(currentMillis-previousMillis>= 2000){    // if >2 seconds from last reading. DHT22 sampling rate is 0.5Hz
    temperature = dht.readTemperature();  // read temperature
    previousMillis=currentMillis;             // reset previousMillis
  }

  if(Serial1.available()){              // run code only if data received from master
    char tempChar[5];                 // char array that will be used to send sensor data to master in bytes
    delay(1);                           // allow time for message to be received in full 
    
  
    if (Serial1.read()=='T'){          // if request for temperature
	  analogWrite(GREEN_PIN, 0);   		// apagar led verde
	  analogWrite(RED_PIN, 255);       		// encender led rojo
      String temperatureRead = String(temperature, 1);  // convert data to String
      temperatureRead.toCharArray(tempChar, 5);  // String sensor reading to char array
      Serial1.write(tempChar, 4);                // send char array to master
      delay(500);
	  analogWrite(GREEN_PIN, 255);    		// encender el led verde
	  analogWrite(RED_PIN, 0);   			// apagar el led rojo 
    } 
  }
}

