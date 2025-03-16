#include <WiFi.h>   // Include WiFi library

#define DELAY 10000 // Delay between network scans

void setup() {   
    Serial.begin(9600);   // Initialize serial communication

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
}            

void loop() {
    Serial.println("Scanning Networks");
    
    int n = WiFi.scanNetworks();	// Scan for available networks 
    if (n == 0) {
        Serial.println("No networks found");
    } else {
        Serial.print("Total networks: ");
        Serial.println(n);
        
        for (int i=0; i<n; i++){		 // Loop through all found networks
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(WiFi.SSID(i));	// Display network name
            Serial.print(" (");
            enc_type(WiFi.encryptionType(i));	// Display encryption type
            Serial.println(")");
            delay(10);
	    }
    }

    delay(DELAY);                 
}

void enc_type(int type) {
    // Convert encryption type constant to readable text
    switch (type) {
        case WIFI_AUTH_OPEN:
            Serial.print("open");
            break;
        case WIFI_AUTH_WEP:
            Serial.print("WEP");
            break;
        case WIFI_AUTH_WPA_PSK:
            Serial.print("WPA");
            break;
        case WIFI_AUTH_WPA2_PSK:
            Serial.print("WPA2");
            break;
        case WIFI_AUTH_WPA_WPA2_PSK:
            Serial.print("WPA+WPA2");
            break;
        case WIFI_AUTH_WPA2_ENTERPRISE:
            Serial.print("WPA2-EAP");
            break;
        case WIFI_AUTH_WPA3_PSK:
            Serial.print("WPA3");
            break;
        case WIFI_AUTH_WPA2_WPA3_PSK:
            Serial.print("WPA2+WPA3");
            break;
        case WIFI_AUTH_WAPI_PSK:
            Serial.print("WAPI");
            break;
        default:
            Serial.print("unknown");
    }
}
