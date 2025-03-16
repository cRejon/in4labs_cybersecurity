#include <WiFi.h>   // Include WiFi library

#define DELAY 10000 // Wait time between scans


void setup() {   
    Serial.begin(9600);   // Initialize serial port

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
}            

void loop() {
    Serial.println("Scanning Networks");
    
    // your code here


    delay(DELAY);  
}

void enc_type(int type) {
  // Method to convert the constant value indicating encryption type to text
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
