#include <Wire.h> 			// incluye la libreria Wire
#include <DHT.h>  			// incluye la libreria DHT
#include "DES.h" 			//incluye la libreria DES

#define RED_PIN A0  
#define GREEN_PIN A1
#define BLUE_PIN A2

#define DATA_PIN 2                   // define the type data pin
#define DHT_TYPE DHT22               // define the DHT sensor (DHT11, DHT21, or DHT22)

DHT dht = DHT(DATA_PIN, DHT_TYPE);   // instantiate the dht class with our data pin and DHT type

DES des;						// declara la variable para encritar y desencriptar
byte key[] = { 
                  0x3b, 0x38, 0x98, 0x37, 0x15, 0x20, 0xf7, 0x5e, // Clave A
                  0x92, 0x2f, 0xb5, 0x10, 0xc7, 0x1f, 0x43, 0x6e, // Clave B
                  0x3b, 0x38, 0x98, 0x37, 0x15, 0x20, 0xf7, 0x5e, // Clave C 
                };				// Define la clave triple de 64 bits pot fila para usar 3DES

void setup() {
  dht.begin();

  Wire.begin(9);					// estableciendo dirección 9
  Wire.setClock(10000);				// establezco la velocidad del bus en lenta
  Wire.onReceive(receiveEvent);		// si se recibe algún dato pasarlo a la funcion 'receiveEvent'
  Wire.onRequest(receptionEvent); 	// si se requiere algun dato ejecutar la funcion 'receptionEvent'

  pinMode(GREEN_PIN, OUTPUT);	// Led verde encendido significa que el puerto serie esta libre
  pinMode(BLUE_PIN, OUTPUT);   // Led azul no se usará de momento
  pinMode(RED_PIN, OUTPUT);    // Led rojo encendido significa que el puerto serie esta ocupado
  analogWrite(GREEN_PIN, 255);  // encender led verde
  analogWrite(BLUE_PIN, 0); // apagar led azul  
  analogWrite(RED_PIN, 0);	// apagar led rojo
}

String temperatureRead;				// variable para guardar la lectura del sensor+
byte msg[8];                        // almacenará el mensaje cifrado a recibir y enviar
byte texto[8];						// almacena la cadena en texto plano a enviar o recibir

void loop() {
  delay(1000);            		//Espera activa de 1 segundo
}

void receptionEvent(){
	analogWrite(GREEN_PIN, 0);  	// apagar led verde
	analogWrite(BLUE_PIN, 255);       	// encender led rojo
	String temp;
	temp=String(temperatureRead.toInt());				// paso a cadena un entero de un float
	temp.toCharArray(texto, temp.length()+1);			// paso la cadena a una tabla
	des.tripleEncrypt(msg,texto,key); 					// Encriptar el mensaje
	Wire.write(msg,8);							// escribir 8 bytes
	analogWrite(GREEN_PIN, 255);    	// encender el led verde
	analogWrite(BLUE_PIN, 0);   	// apagar el led rojo
}

void receiveEvent(int numBytes){
  analogWrite(GREEN_PIN, 0);  	// apagar led verde
  analogWrite(RED_PIN, 255);       	// encender led rojo
  if (Wire.available()){				// hay datos que leer
	//HAY QUE LEER 8 BYTES
	for (int i = 0; i < 8; i++) {		// leer 8 bytes
		msg[i]= Wire.read();			// que corresponde con la cadena encriptada
	}
	String cadena_descifrada;			// almacena el texto plano descifrado
	cadena_descifrada="";
	des.tripleDecrypt(texto, msg, key);	// desencripta la cadena de byte msg en texto
	for (int i = 0; i < 2; i++) {
		cadena_descifrada += String((char)texto[i]);
	}
	if (cadena_descifrada=="T"){        // si se ha recibido una T connsulto la temperatura    	 
		temperatureRead = dht.readTemperature(); 	
	}
  }
  analogWrite(GREEN_PIN, 255);    	// encender el led verde
  analogWrite(RED_PIN, 0);   	// apagar el led rojo	
}