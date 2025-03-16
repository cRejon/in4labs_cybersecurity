#include <DHT.h>  // Include DHT sensor library

// RS-485 pins
#define TX_PIN 7                  
#define RX_PIN 8  

#define DATA_PIN 2                   // Define the data pin
#define DHT_TYPE DHT22               // Define the DHT sensor (DHT11, DHT21, or DHT22)

#define DELAY 20000                  // Define the delay between sensor readings


DHT dht = DHT(DATA_PIN, DHT_TYPE);   // Instantiate the DHT class with our data pin and DHT type.

void setup() {
    Serial1.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);  // Initialize serial port
    while (!Serial1) {
        ; // Wait for serial port connection
    }

    dht.begin();                
}

long previousMillis = 0;    // Variable to track the last sensor reading time

void loop() {
    long currentMillis = millis();              
    if(currentMillis-previousMillis >= DELAY){    // Check if 20 seconds have elapsed since last reading
        float temperature = dht.readTemperature();  // Read temperature from sensor    
        int intTemperature = (int)temperature;
        Serial1.write(intTemperature);             // Send temperature over serial port
        Serial1.flush();                           // Clear output buffer
        previousMillis = currentMillis;            // Update last reading timestamp
    }
}
