#include <WiFi.h>     // Include the WiFi library
#include <DHT.h>      // Include the DHT sensor library

#define RED_PIN A0  
#define GREEN_PIN A1
#define BLUE_PIN A2

#define DATA_PIN 2
#define DHT_TYPE DHT22

DHT dht = DHT(DATA_PIN, DHT_TYPE);

char ssid[] = "WiFi-Access-Point";
char pass[] = "123456789";
int keyIndex = 0;

int status = WL_IDLE_STATUS;

WiFiServer server(80, 1);

void setup() {
    dht.begin();

    status = WiFi.softAP(ssid, pass);

    if (status != WL_AP_LISTENING) {
        while (true);
    }

    server.begin();

    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);
    pinMode(RED_PIN, OUTPUT);
    analogWrite(GREEN_PIN, 255);
    analogWrite(BLUE_PIN, 0);
    analogWrite(RED_PIN, 0);
}

void loop() {
    String sensorRead = "";

    WiFiClient client = server.available(0);
    if (client) {
        String currentLine = "";
        boolean currentLineIsBlank = true;
        while (client.connected()) {
            analogWrite(GREEN_PIN, 0);
            analogWrite(BLUE_PIN, 255);
            if (client.available()) {
                char c = client.read();

                if (c == '\n' && currentLine.length() == 0) {
                    if (sensorRead != 0) {
                        client.print(sensorRead);
                        client.println();
                        sensorRead = "";
                    }
                    break;
                }
                if (c == '\n') {
                    currentLine = "";
                } else if (c != '\r') {
                    currentLine += c;
                }

                if (currentLine.endsWith("GET /Temperature")) {
                    float temp = dht.readTemperature(true);
                    sensorRead = String(temp, 2);
                }
            }
        }
        delay(1);

        client.stop();
        analogWrite(GREEN_PIN, 255);
        analogWrite(BLUE_PIN, 0);
    }
}
