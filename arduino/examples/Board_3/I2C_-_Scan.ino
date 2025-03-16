#include <Wire.h>           // Include Wire library for I2C communication

#define FAN_PIN 2          // Pin connected to the fan control transistor
#define TEMP_LIMIT 25      // Temperature threshold for fan activation
#define DELAY 20000        // Delay between temperature readings

void setup() {
    Wire.begin(10);          // Set device address to 10

    pinMode(FAN_PIN, OUTPUT);        // Configure fan pin as output
    digitalWrite(FAN_PIN, LOW);      // Initially turn fan off
}

int temperature;           // Variable to store temperature
long previousMillis = 0;

void loop() {
    long currentMillis = millis();              
    
    if (currentMillis-previousMillis >= DELAY){    // Check temperature every 20 seconds
        previousMillis=currentMillis;
        
        Wire.beginTransmission(9);     // Start transmission to slave address 9
        Wire.write('T');               // Send temperature request command
        Wire.endTransmission();        // End transmission
        delay(1000);                    // Wait for temperature update
        
        Wire.requestFrom(9, 1);        // Request 1 byte from slave device
        while (Wire.available()){         // If data is available on the bus
            char c = Wire.read();      // Read one byte
            temperature = int(c);
            if (temperature > TEMP_LIMIT){     // If temperature exceeds limit
                digitalWrite(FAN_PIN, HIGH);  // Turn fan on   
            }else{
                digitalWrite(FAN_PIN, LOW);   // Turn fan off
            }
        }
    }
}