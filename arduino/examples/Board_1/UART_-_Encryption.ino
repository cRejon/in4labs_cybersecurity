#include <DHT.h>                    // include the DHT library
#include "AESLib.h"                 // include AES library for encryption

#define DATA_PIN 2                  // define the data pin for the sensor
#define DHT_TYPE DHT22              // define the DHT sensor type (DHT11, DHT21, or DHT22)

// AES Encryption Key (must be same on both devices)
byte aes_key[] = { 0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C };

// Initialization vector 
byte aes_iv[N_BLOCK] = { 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA };

AESLib aesLib;
unsigned char ciphertext[32]; // buffer for encrypted data

DHT dht = DHT(DATA_PIN, DHT_TYPE);    // instantiate the DHT sensor using the data pin and sensor type

void setup() {
  Serial0.begin(9600);          // set serial port baud rate
  while (!Serial0) {
      ; // wait for serial port connection
  }    // turn off red LED

  dht.begin();

  aesLib.gen_iv(aes_iv);
  aesLib.set_paddingmode((paddingMode)0);
}

long previousMillis = 0;    // last time the sensor was checked (in ms)
float temperature;          // variable to hold data read from the sensor

// Function to encrypt temperature data
uint16_t encryptTemperature(int temp, byte iv[]) {
  char tempStr[4];
  sprintf(tempStr, "%d", temp); // Convert int to string
  
  // Encrypt the temperature string
  int cipherlen = aesLib.encrypt((byte*)tempStr, strlen(tempStr), ciphertext, aes_key, sizeof(aes_key), iv);
  return cipherlen;
}

void loop() {
  long currentMillis = millis();              // current time for comparison
  
  if (currentMillis - previousMillis >= 2000) { // if more than 2 seconds have passed; DHT22 sampling rate is 0.5Hz
    temperature = dht.readTemperature();        // read the temperature
    previousMillis = currentMillis;             // update last reading time
  }

  delay(100); // Small delay to avoid overwhelming the Serial output
  if (Serial0.available()) {              // execute when data is received from the master
    
    if (Serial0.read() == 'T') {          // if the request is for temperature
      // Make a local copy of IV to use for encryption
      byte enc_iv[N_BLOCK];
      memcpy(enc_iv, aes_iv, sizeof(aes_iv));
      
      int intTemperature = (int)temperature;    // convert float to integer
      uint16_t encLen = encryptTemperature(intTemperature, enc_iv);
      
      // Send encrypted data length first
      Serial0.write((byte)encLen);
      delay(50);
      
      // Then send the encrypted data
      Serial0.write(ciphertext, encLen);
      delay(500);
    } 
  }
}

