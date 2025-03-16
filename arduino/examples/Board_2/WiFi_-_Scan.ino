#include <WiFi.h>               // include WiFi library
#include <LiquidCrystal.h>      // include LCD library

#define TIMEOUT 2000           // Tiempo de espera para la respuesta del servidor
#define DELAY 10000             // Tiempo de espera entre lecturas


char ssid[] = "WiFi-Access-Point";        // Define the SSID
char password[] = "123456789";                    // Connection password

char serverSensor[] = "192.168.4.1";    // Server IP address
WiFiClient client;                      // Crear cliente WiFi

int temperature = 0;              // Variable de temperatura

const int rs = 3, en = 4, d4 = 5, d5 = 6, d6 = 9, d7 = 10;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void connectToWiFi() {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
    }
}

// function that sends a request to the server
void sendHttpRequest(const char* server, const char* request) {
    client.print("GET /");
    client.print(request);
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(server);
    client.println("Connection: keep-alive");
    client.println();     // send empty new line (end of HTTP request)
    delay(1);            // give client time to receive, process data and reply
}

// Refactored readHttpResponse function using millis() for timeout checking
String readHttpResponse() {
    String response;
    unsigned long startTime = millis();
    const unsigned long timeout = TIMEOUT;
    // Wait for data to become available (up to 1s)
    while (!client.available() && (millis() - startTime < timeout)) {
        delay(10);
    }
    // Read available data
    while (client.available()) {
        response += (char)client.read();
    }

    client.stop();
    return response;
}

void updateLCD() {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Temp: ");
    lcd.print(temperature);
    lcd.print(" C");
} 

void setup() {
    IPAddress local_IP(192, 168, 4, 2);
    IPAddress gateway(192, 168, 4, 1);
    IPAddress subnet(255, 255, 255, 0);
    WiFi.config(local_IP, gateway, subnet);  // Se configura IP del cliente
    connectToWiFi();

    lcd.begin(16, 2);           // set up the LCD's number of columns and rows
    lcd.clear();
    lcd.print("Setup done");   
    delay(2000);
}

void loop() {
    
    if (client.connect(serverSensor, 80)) {   // Si hay conexion con el servidor en el puerto 80
        String dataRead; 
        sendHttpRequest(serverSensor, "temperature");  // Enviar solicitud de temperatura                                   
        dataRead = readHttpResponse(); 
        temperature = dataRead.toInt(); // Convertir la temperatura a entero
        updateLCD();
    }

    delay(DELAY);                     
}
