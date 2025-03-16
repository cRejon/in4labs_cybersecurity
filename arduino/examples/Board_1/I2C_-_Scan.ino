#include <Wire.h> 				// includes the Wire library
#include <DHT.h>  				// includes the DHT library

#define DATA_PIN 2                   // defines the data pin
#define DHT_TYPE DHT22               // defines the DHT sensor (DHT11, DHT21, or DHT22)

DHT dht = DHT(DATA_PIN, DHT_TYPE);   // instantiate the DHT class with our data pin and DHT type.

void setup() {
    dht.begin(); 

    Wire.begin(9);					// setting address 9
    Wire.onReceive(receiveEvent);		// if data is received, pass it to the 'receiveEvent' function
    Wire.onRequest(requestEvent); 	// if data is requested, execute the 'requestEvent' function
}

int temperature;				// variable to store the sensor reading

void loop() {
    delay(2000);            		// active wait for 2 seconds
}

void requestEvent(){
    Wire.write(temperature);  // send a byte with the temperature value
}

void receiveEvent(int numBytes){
    if (Wire.available()){				// there is data to read
        if (Wire.read()=='T'){            	 
            float temp = dht.readTemperature(); 	
            temperature = (int)temp;
        }
    }
}