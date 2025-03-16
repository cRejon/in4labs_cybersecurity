#include <DHT.h>                    // include the DHT library

#define RED_PIN A0  
#define GREEN_PIN A1
#define BLUE_PIN A2

#define DATA_PIN 2                  // define the data pin for the sensor
#define DHT_TYPE DHT22              // define the DHT sensor type (DHT11, DHT21, or DHT22)

DHT dht = DHT(DATA_PIN, DHT_TYPE);    // instantiate the DHT sensor using the data pin and sensor type

void setup() {
  Serial0.begin(9600);          // set serial port baud rate
  while (!Serial0) {
      ; // wait for serial port connection
  }

  pinMode(GREEN_PIN, OUTPUT);   // Green LED on indicates the serial port is free
  pinMode(BLUE_PIN, OUTPUT);    // Blue LED is not used currently
  pinMode(RED_PIN, OUTPUT);     // Red LED on indicates the serial port is busy
  analogWrite(GREEN_PIN, 255);   // turn on green LED
  analogWrite(BLUE_PIN, 0);      // turn off blue LED  
  analogWrite(RED_PIN, 0);       // turn off red LED

  dht.begin(); 
}

long previousMillis = 0;    // last time the sensor was checked (in ms)
float temperature;          // variable to hold data read from the sensor

void loop() {
  long currentMillis = millis();              // current time for comparison
  
  if (currentMillis - previousMillis >= 2000) { // if more than 2 seconds have passed; DHT22 sampling rate is 0.5Hz
    temperature = dht.readTemperature();        // read the temperature
    previousMillis = currentMillis;             // update last reading time
  }

  delay(100); // Small delay to avoid overwhelming the Serial output
  if (Serial0.available()) {              // execute when data is received from the master
    //delay(100);                           // allow time for the complete message to be received 
    
    if (Serial0.read() == 'T') {          // if the request is for temperature
      analogWrite(GREEN_PIN, 0);         // turn off green LED
      analogWrite(RED_PIN, 255);         // turn on red LED
      int intTemperature = (int)temperature;    // convert float to integer
      Serial0.write((byte)intTemperature);      // send the integer as a byte
      delay(500);
      analogWrite(GREEN_PIN, 255);       // turn on green LED
      analogWrite(RED_PIN, 0);           // turn off red LED 
    } 
  }
}

