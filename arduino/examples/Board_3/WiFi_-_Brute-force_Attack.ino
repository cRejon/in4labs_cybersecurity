#include <WiFi.h>   


int status = WL_IDLE_STATUS;
const char* ssid = "WiFi-Access-Point";  // Target SSID to connect to
const char* passwords[] = {"admin","adminttd","1234","admin1234","123456789","123456","admin123456789"};	// List of passwords to try
int numPasswords = sizeof(passwords) / sizeof(passwords[0]);

void setup() {  
    Serial.begin(9600);   // Initialize serial communication
    Serial.println("Brute-force Attack");

    IPAddress local_ip(192, 168, 4, 3);
    IPAddress gateway(192, 168, 4, 1);
    IPAddress subnet(255, 255, 255, 0);
    WiFi.config(local_ip, gateway, subnet);
    
    for (int i=0; i < numPasswords; i++) {
        Serial.print("Trying password: ");
        Serial.println(passwords[i]);
        // your code here



        
    }
    
    if (status != WL_CONNECTED) {
        Serial.println("Could not connect with any of the passwords");
    }
}

void loop() {
    // Do nothing
    delay(1000);
}

