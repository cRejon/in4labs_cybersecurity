#include <DHT.h>  // Include DHT sensor library

// RS485 pins
#define TX_PIN 7                  
#define RX_PIN 8  

#define DATA_PIN 2                   // Define the data pin
#define DHT_TYPE DHT22               // Define the DHT sensor (DHT11, DHT21, or DHT22)


DHT dht = DHT(DATA_PIN, DHT_TYPE);   // Instantiate the DHT class with our data pin and DHT type.

void setup() {
    Serial1.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);  // Initialize serial port
    while (!Serial1) {
        ; // Wait for serial port connection
    }

    dht.begin();                
}

float temperature;     // Variable to store sensor reading

void loop() {
                 
    if (Serial1.available()){                   // Check if data is available on the bus
        delay(100);
        if (Serial1.read()=='I'){              // Detect frame start
            if (Serial1.read()=='T'){          // Check if it's a temperature request
                if (Serial1.read()=='F'){      // Verify frame end - frame is valid
                    temperature = dht.readTemperature();  // Read temperature
                    Serial1.write("I");
                    Serial1.write(int(temperature));
                    Serial1.write("F");
                    Serial1.flush();
                    delay(100);
                }
            }
        }
    }
}
