#include <SPI.h>
#include <WiFiNINA.h>	//Declaro la libreria WiFi
#include <Wire.h>   	//Icluir libreria para la pantalla

#define fanPin 10       // Se define el pin del ventilador

#define temperatureLimit 23.00  //Limite de temperatura

#define redStatusPin 5        		// Define pin led rojo
#define greenStatusPin 6      		// Define pin led verde
#define blueStatusPin 9       		// Define pin led azul

#define controlSPIBusClock 4  

char ssid[] = "WifiNINA-Access-Point";        // Se define el SSID
char pass[] = "123456789";                    // Contraseña de conexión
int keyIndex = 0;                             // Key-Index solo para WEP

int status = WL_IDLE_STATUS;

char serverSensor[] = "192.168.4.1";    	// IP del servidor

WiFiClient client;							// Se define el cliente

void setup() {
  Wire.begin(2);               
  
  pinMode(controlSPIBusClock, OUTPUT);    
  digitalWrite(controlSPIBusClock, LOW);  

  while (status != WL_CONNECTED) {        // Si el estado es no conectado
    status = WiFi.begin(ssid, pass);      // Intendo conectar con el punto de acceso
    delay(1000);                          // Espero un segundo
  }
  
  WiFi.config(IPAddress(192, 168, 4, 2));	//Se configura IP del cliente
  
  pinMode(greenStatusPin, OUTPUT);      // Se define el pin del led verde como salida
  pinMode(blueStatusPin, OUTPUT);       // Se define el pin del led azul como salida
  pinMode(redStatusPin, OUTPUT);        // Se define el pin del led rojo como salida
  digitalWrite(greenStatusPin, 0);      // Se enciende el led verde
  digitalWrite(blueStatusPin, 255);     // Se apaga el led azul
  digitalWrite(redStatusPin, 255);		// Se apaga el led rojo

  digitalWrite(fanPin, 0);      		// Ventilador apagado
}



void loop() {
         
  
  if (client.connect(serverSensor, 80)) {   // Si estoy conectado al puerto 80
    digitalWrite(greenStatusPin, 250);      // Apago el led verde
    digitalWrite(blueStatusPin,0);          // Enciendo el led azul

    while (1){								// Bucle infinito
		if (!client.connected()) {          // Si me cierran la conexion
			client.connect(serverSensor, 80);	//Vuelvo a conectarme
		}
		client.print("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");	//Envio una cadena sin retorno
	}
    digitalWrite(greenStatusPin, 0);      	// Enciendo el led verde
    digitalWrite(blueStatusPin,255);        // Apago el led azul
  }

  delay(2000);                    
}
