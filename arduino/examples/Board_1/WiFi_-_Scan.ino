#include <WiFi.h>     // Include the WiFi library
#include <DHT.h>      // Include the DHT sensor library

#define RED_PIN A0  
#define GREEN_PIN A1
#define BLUE_PIN A2

#define DATA_PIN 2                   // Define the data pin for the DHT sensor
#define DHT_TYPE DHT22               // Define the DHT sensor type (DHT11, DHT21, or DHT22)

DHT dht = DHT(DATA_PIN, DHT_TYPE);   // Instantiate the DHT class with the data pin and DHT type

char ssid[] = "WiFi-Access-Point";        // Define the SSID
char pass[] = "123456789";                    // Connection password
int keyIndex = 0;                             // Key-Index only for WEP

int status = WL_IDLE_STATUS;

WiFiServer server(80);                        // Define the server listening on port 80

void setup() {
    dht.begin();                

    // By default, the local IP is 192.168.4.1
    // If you want to change it, uncomment the following line
    // WiFi.config(IPAddress(10, 0, 0, 1));
    WiFi.begin(ssid, pass);                     // Connect to WiFi network
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
    
    server.begin();                              // Initialize the server

    pinMode(GREEN_PIN, OUTPUT);      // Se define el pin del led verde como salida
    pinMode(BLUE_PIN, OUTPUT);       // Se define el pin del led azul como salida
    pinMode(RED_PIN, OUTPUT);        // Se define el pin del led rojo como salida
    analogWrite(GREEN_PIN, 255);      // Se enciende el led verde
    analogWrite(BLUE_PIN, 0);     // Se apaga el led azul
    analogWrite(RED_PIN, 0);    // Se apaga el led rojo
}

void loop() {
    String sensorRead = "";                        // Variable to store the sensor temperature
    
    WiFiClient client = server.available();   // Check if there are any connected clients
    if (client) {                             // If there is a client
        String currentLine = "";                // Set the current line to empty
        boolean currentLineIsBlank = true;      // Set the current line empty flag to true             
        while (client.connected()) {            // While the client is connected
            analogWrite(GREEN_PIN, 0);    // Turn off the green LED
            analogWrite(BLUE_PIN, 255);        // Turn on the blue LED
            if (client.available()) {             // If there is data to read from the client
                char c = client.read();             // Read byte
                
                if (c == '\n' && currentLine.length() == 0) {	// If a newline is read and the current line is empty
                    
                    if (sensorRead != ""){         // If the sensor has a value  
                        client.print(sensorRead);   // Send the sensor value to the client      
                        client.println();           // Send an empty line 
                        sensorRead = "";            // Clear the read value
                    }
                    break;
                }
                if (c == '\n') {                // If a newline is read
                    currentLine = "";				// Clear the line
                } else if (c != '\r') {         // If the read character is not a carriage return
                    currentLine += c;             // Add the character to the line
                } 
                
                if (currentLine.endsWith("GET /Temperature")){      // If the line ends with a temperature request
                    float temp = dht.readTemperature();             // Read the sensor
                    sensorRead = String(temp, 2);               
                }
            }
        }
        delay(1);    

        client.stop();                      // Close the connection
        analogWrite(GREEN_PIN, 255);    // Turn on the green LED
        analogWrite(BLUE_PIN, 0);   // Turn off the blue LED
    }
}
