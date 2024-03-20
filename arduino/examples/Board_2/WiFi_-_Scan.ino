#include <WiFiNINA.h>	//Declaro la libreria WiFi
#include <DHT.h>    	//Incluir libreria del sensor de temperatura

#define RED_PIN 5  
#define GREEN_PIN 6
#define BLUE_PIN 9

#define DATA_PIN 2                   // define the type data pin
#define DHT_TYPE DHT22               // define the DHT sensor (DHT11, DHT21, or DHT22)

DHT dht = DHT(DATA_PIN, DHT_TYPE);   // instantiate the dht class with our data pin and DHT type

char ssid[] = "WifiNINA-Access-Point";        // Se define el SSID
char pass[] = "123456789";                    // Contraseña de conexión
int keyIndex = 0;                             // Key-Index solo para WEP

int status = WL_IDLE_STATUS;

WiFiServer server(80);        				// Define el servicio de escucha por el puerto 80

void setup() {
  dht.begin();                

  // Por defecto la IP local es 192.168.4.1
  // Si se quisiera cambiar defina la siguiente linea
  // WiFi.config(IPAddress(10, 0, 0, 1));
  status = WiFi.beginAP(ssid, pass);		//Se crea el punto de acceso
  
  if (status != WL_AP_LISTENING) {
    //Si da error al crearlo no seguir
    while (true);
  }
  
  server.begin();              				// Inicializar servidor

  pinMode(GREEN_PIN, OUTPUT);      // Se define el pin del led verde como salida
  pinMode(BLUE_PIN, OUTPUT);       // Se define el pin del led azul como salida
  pinMode(RED_PIN, OUTPUT);        // Se define el pin del led rojo como salida
  digitalWrite(GREEN_PIN, 255);      // Se enciende el led verde
  digitalWrite(BLUE_PIN, 0);     // Se apaga el led azul
  digitalWrite(RED_PIN, 0);		// Se apaga el led rojo
}


void loop() {
  String sensorRead="";                        // VAriable de lectura de temp del sensor
  
  WiFiClient client = server.available();   // Viendo si hay clientes conectados
  if (client) {                             // Si hay un cliente
    String currentLine = "";                // Establezco la linea actual a vacia
    boolean currentLineIsBlank = true;      // Establezco la variable de linea vacia a true             
    while (client.connected()) {            // Mientras el cliente este conectado
      digitalWrite(GREEN_PIN, 0);    // Apago el led verde
      digitalWrite(BLUE_PIN, 255);        // Enciendo el led azul
      if (client.available()) {             // Si hay datos para leer del cliente
        char c = client.read();             // Leer byte
        
        if (c == '\n' && currentLine.length() == 0) {	//Si se lee un salto de linea y la linea actual es de longitus cero significa que no hay nada mas que recibir
          
          if (sensorRead != 0){         // Si el sensor tiene valor  
            client.print(sensorRead);   // Envio el valor del sensor al cliente      
            client.println();           // Envio una linea vacia 
            sensorRead = "";            // Borro el valor leido
          }
          break;
        }
        if (c == '\n') {                // Si se lee un final de linea
          currentLine = "";				// vacio la linea
        } else if (c != '\r') {         // Si el caracter leido es distinto a un retorno de carro
          currentLine += c;             // Añado el caracter a la linea
        } 
        
        if (currentLine.endsWith("GET /Temperature")){      // Si la linea termina en una peticion de temperatura
          float temp = dht.readTemperature();             // Leo el sensor
          sensorRead = String(temp, 2);               
        }
      }
    }
    delay(1);    

    client.stop();                      // Cierro la conexion
    digitalWrite(GREEN_PIN, 255);    // Enciendo el led verde
    digitalWrite(BLUE_PIN, 0);   // Apago el led azul
  }
}
