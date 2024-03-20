#include <WiFiNINA.h>	//Declaro la libreria WiFi

#define FAN_PIN 2             	// Pin de conexión del transistor que activa el ventilador
#define TEMP_LIMIT 25         	// Límite de temperatura

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

String temperatureRead;               // Cadena de temperatura 
char c;                               // Caracter de lectura

void loop() {
  int counter = 0;            
  temperatureRead = "";       
  
  if (client.connect(serverSensor, 80)) {   // Si hay conexion con el servidor en el puerto 80
    digitalWrite(GREEN_PIN, 0);      // Apago el led verde
    digitalWrite(BLUE_PIN, 255);          // Enciendo el led azul

    client.println("GET /Temperature HTTP/1.1");  // Hacer una peticion de temperatura
    client.println("Host: 192.168.4.1");          
    client.println("Connection: keep-alive");
    client.println();                             // Enviar una cadena vacia para decir que ha terminado el envio
    delay(1);                                     
    
    while (client.available() == 0 && counter < 100 ){  //Esperar a que el cliente mande algun valor
      delay(10);                    
      counter = counter+1;          
      if  (counter == 100){         
        temperatureRead = "error";  
      }
    }

    if (counter != 100){                // Si el contador no llego al final significa que hay valores que leer
      while (client.available() > 0){   // Mientras haya datos a leer
        c = client.read();              // Leer byte
        temperatureRead += c;           // Añador caracter a la cadena de temperatura 
      }  
    }
    counter = 0;                        
    
    if (!client.connected()) {          // Si el cliente no esta conectado
      client.stop();                    // Para la conexion
    }  


    digitalWrite(GREEN_PIN, 255);    //Encender led verde
    digitalWrite(BLUE_PIN, 0);   //Apagar led azul

    delay(1);                           
  }

  float temperatureFloat = temperatureRead.toFloat(); //Convertir la temperatura para comparar

  if (temperatureFloat > TEMP_LIMIT){ 
    analogWrite (FAN_PIN, 256);                //Encender ventilador
  }
  else {
    analogWrite (FAN_PIN, 0);                  //Apagar ventilador
  }

  delay(2000);                     
}
