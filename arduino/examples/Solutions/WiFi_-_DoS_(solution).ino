#include <WiFi.h>	//Declaro la libreria WiFi

#define FAN_PIN 2             	// Pin de conexión del transistor que activa el ventilador

#define RED_PIN A0  
#define GREEN_PIN A1
#define BLUE_PIN A2

char ssid[] = "WiFi-Access-Point";        // Se define el SSID
char pass[] = "123456789";                    // Contraseña de conexión
int keyIndex = 0;                             // Key-Index solo para WEP

int status = WL_IDLE_STATUS;

char serverSensor[] = "192.168.4.1";    	// IP del servidor

WiFiClient client;							// Se define el cliente

void setup() {
    while (status != WL_CONNECTED) {        // Si el estado es no conectado
        status = WiFi.begin(ssid, pass);      // Intendo conectar con el punto de acceso
        delay(1000);                          // Espero un segundo
    }
    
    IPAddress local_ip(192, 168, 4, 2);
    IPAddress gateway(192, 168, 4, 1);
    IPAddress subnet(255, 255, 255, 0);
    WiFi.config(local_ip, gateway, subnet);	//Se configura IP del cliente
    
    pinMode(GREEN_PIN, OUTPUT);      // Se define el pin del led verde como salida
    pinMode(BLUE_PIN, OUTPUT);       // Se define el pin del led azul como salida
    pinMode(RED_PIN, OUTPUT);        // Se define el pin del led rojo como salida
    analogWrite(GREEN_PIN, 255);      // Se enciende el led verde
    analogWrite(BLUE_PIN, 0);     // Se apaga el led azul
    analogWrite(RED_PIN, 0);		// Se apaga el led rojo

    digitalWrite(FAN_PIN, LOW);      		// Ventilador apagado
}

void loop() {
    if (client.connect(serverSensor, 80)) {   // Si estoy conectado al puerto 80
        analogWrite(GREEN_PIN, 0);      // Apago el led verde
        analogWrite(BLUE_PIN, 255);          // Enciendo el led azul

        while (1){								// Bucle infinito
            if (!client.connected()) {          // Si me cierran la conexion
                client.connect(serverSensor, 80);	//Vuelvo a conectarme
            }
            client.print("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");	//Envio una cadena sin retorno
        }
        analogWrite(GREEN_PIN, 255);      	// Enciendo el led verde
        analogWrite(BLUE_PIN, 0);        // Apago el led azul
    }

    delay(2000);                    
}
