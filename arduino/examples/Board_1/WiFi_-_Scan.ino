#include <WiFi.h>     // Include the WiFi library
#include <DHT.h>      // Include the DHT sensor library

#define DATA_PIN 2                   // Define the data pin for the DHT sensor
#define DHT_TYPE DHT22               // Define the DHT sensor type (DHT11, DHT21, or DHT22)

DHT dht = DHT(DATA_PIN, DHT_TYPE);   // Instantiate the DHT class with the data pin and DHT type

char ssid[] = "WiFi-Access-Point";        // Define the SSID
char password[] = "123456789";                    // Connection password

WiFiServer server(80);                        // Define the server listening on port 80

// New function to handle an incoming client connection.
void handleClient(WiFiClient &client) {
    String line = "";
    while (client.connected()) {
        if (client.available()) {
            char c = client.read();
            if (c == '\n') {
                // when an empty line is received, the HTTP header is done
                if (line.length() == 0) {
                    // If a temperature request was detected in any line, send the sensor value.
                    // (Alternatively, the GET line can be processed immediately as shown below)
                    break;
                }
                // Check the line for the GET request
                if (line.indexOf("GET /temperature") >= 0) {
                    int temp = int(dht.readTemperature());
                    client.print(String(temp));
                    client.println();
                }
                line = "";
            } else if (c != '\r') {
                line += c;
            }
        }
    }
    delay(1);
    client.stop();
}

void setup() {
    dht.begin();                

    // Create open network, by default the local IP address of will be 192.168.4.1
    if (!WiFi.softAP(ssid, password)) { 
        // Creating access point failed, don't continue
        while (true); // Halt the program
    }  
    
    server.begin();                              // Initialize the server
}

void loop() {
    WiFiClient client = server.available();   // Check for an available client
    if (client) {
        handleClient(client);
    }
}
