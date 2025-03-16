#include <WiFi.h>	//Include WiFi library

#define RED_PIN A0  
#define GREEN_PIN A1
#define BLUE_PIN A2


char ssid[] = "WiFi-Access-Point";        // Define the SSID
char pass[] = "123456789";                // Connection password

char serverSensor[] = "192.168.4.1";    	// Server IP

WiFiClient client;							// Define the client

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
    WiFi.config(local_ip, gateway, subnet);	//Configure client IP
    
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
    }
}

void loop() {
    if (client.connect(serverSensor, 80)) {   // If connected to port 80
        digitalWrite(GREEN_PIN, LOW);    
        digitalWrite(RED_PIN, HIGH);  

        // your code here
        

        

        digitalWrite(RED_PIN, LOW);        
        digitalWrite(GREEN_PIN, HIGH);  
    }

    delay(2000);                    
}
