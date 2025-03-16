#include <Wire.h> 			            // include the Wire library
#include <DHT.h>  			            // include the DHT library
#include <DES.h>                        // include the DES library for encryption

#define DATA_PIN 2                      // define the data pin
#define DHT_TYPE DHT22                  // define the DHT sensor (DHT11, DHT21, or DHT22)

// Define encryption key (24 bytes for Triple DES)
byte encryptionKey[24] = {
  0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
  0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10,
  0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88
};

// Buffers for encryption
byte plainData[8] = {0};
byte encryptedData[8] = {0};

DHT dht = DHT(DATA_PIN, DHT_TYPE);      // instantiate the DHT class with our data pin and DHT type
DES des;                                // create DES object for encryption

void setup() {
    dht.begin(); 
    des.init(encryptionKey);            // Initialize DES with our key

    Wire.begin(9);				        // setting address 9
    Wire.onReceive(receiveEvent);		// if data is received, pass it to the 'receiveEvent' function
    Wire.onRequest(requestEvent); 	    // if data is requested, execute the 'requestEvent' function
}

int temperature;				        // variable to store the sensor reading

void loop() {
    delay(2000);            		    // Active wait for 2 seconds
}

void requestEvent(){
    // Clear the buffers
    memset(plainData, 0, 8);
    memset(encryptedData, 0, 8);
    
    // Put temperature in the first byte of data
    plainData[0] = (byte)temperature;
    
    // Encrypt the data using Triple DES
    des.tripleEncrypt(encryptedData, plainData, encryptionKey);
    
    // Send the encrypted data (8 bytes)
    Wire.write(encryptedData, 8);
}

void receiveEvent(int numBytes){
    if (Wire.available()){				// there is data to read
        if (Wire.read()=='T'){            	 
            float temp = dht.readTemperature(); 	
            temperature = (int)temp;
        }
    }
}