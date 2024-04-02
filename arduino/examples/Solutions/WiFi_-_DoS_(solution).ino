#include <WiFiNINA.h>	//Declaro la libreria WiFi

#define FAN_PIN 2             	// Pin de conexión del transistor que activa el ventilador

#define RED_PIN 5  
#define GREEN_PIN 6
#define BLUE_PIN 9

char ssid[] = "WifiNINA-Access-Point";        // Se define el SSID
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
  
  WiFi.config(IPAddress(192, 168, 4, 2));	//Se configura IP del cliente
  
  pinMode(GREEN_PIN, OUTPUT);      // Se define el pin del led verde como salida
  pinMode(BLUE_PIN, OUTPUT);       // Se define el pin del led azul como salida
  pinMode(RED_PIN, OUTPUT);        // Se define el pin del led rojo como salida
  digitalWrite(GREEN_PIN, 255);      // Se enciende el led verde
  digitalWrite(BLUE_PIN, 0);     // Se apaga el led azul
  digitalWrite(RED_PIN, 0);		// Se apaga el led rojo

  digitalWrite(FAN_PIN, HIGH);      		// Ventilador apagado
}

void loop() {
  if (client.connect(serverSensor, 80)) {   // Si estoy conectado al puerto 80
    digitalWrite(GREEN_PIN, 0);      // Apago el led verde
    digitalWrite(BLUE_PIN, 250);          // Enciendo el led azul

    while (1){								// Bucle infinito
		if (!client.connected()) {          // Si me cierran la conexion
			client.connect(serverSensor, 80);	//Vuelvo a conectarme
		}
		client.print("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");	//Envio una cadena sin retorno
	}
    digitalWrite(GREEN_PIN, 255);      	// Enciendo el led verde
    digitalWrite(BLUE_PIN, 0);        // Apago el led azul
  }

  delay(2000);                    
}
