#include <Wire.h> 			// incluye la libreria Wire
#include <LiquidCrystal.h>      // include LCD library
#include "DES.h" 			//incluye la libreria DES

#define RED_PIN A0  
#define GREEN_PIN A1
#define BLUE_PIN A2

DES des;						// declara la variable para encritar y desencriptar
byte key[] = { 
                  0x3b, 0x38, 0x98, 0x37, 0x15, 0x20, 0xf7, 0x5e, // Clave A
                  0x92, 0x2f, 0xb5, 0x10, 0xc7, 0x1f, 0x43, 0x6e, // Clave B
                  0x3b, 0x38, 0x98, 0x37, 0x15, 0x20, 0xf7, 0x5e, // Clave C 
                };				// Define la clave triple de 64 bits pot fila para usar 3DES

void setup() {
  Wire.begin();
  Wire.setWireTimeout(3000, true);
  
  pinMode(GREEN_PIN, OUTPUT);    	// Led verde encendido significa que el puerto serie esta libre
  pinMode(BLUE_PIN, OUTPUT);     	// Led azul no se usará de momento
  pinMode(RED_PIN, OUTPUT);      	// Led rojo encendido significa que el puerto serie esta ocupado
  analogWrite(GREEN_PIN, 255);    	// encender led verde
  analogWrite(BLUE_PIN, 0);   	// apagar led azul  
  analogWrite(RED_PIN, 0);	// apagar led rojo

  lcd.begin(16, 2);           // set up the LCD's number of columns and rows
  lcd.clear(); 
}

String datos;
int temperature;            // var almacena temperatura
long previousMillis = 0;
byte msg[8];                // almacenará el mensaje cifrado a recibir y enviar
byte texto[8];				// almacena la cadena en texto plano a enviar o recibir
String cadena;

void loop() {
  temperature= 0;       
  long currentMillis = millis();              
  if(currentMillis-previousMillis>= 20000){    	// Si la temperatura se ha leido hace mas de 20seg
    previousMillis=currentMillis;
    analogWrite(GREEN_PIN, 0);  	// apagar led verde
    analogWrite(RED_PIN, 255);       	// encender led rojo
	
	//encriptar cadena a enviar
	cadena="T";						  // se envia un byte concretamente la letra T
	cadena.toCharArray(texto, cadena.length()+1);		// paso la cadena a una tabla
	des.tripleEncrypt(msg,texto,key); // Encriptar el mensaje
    Wire.beginTransmission (9);		// empezar transmisión con el esclavo 9
	Wire.write(msg,8);		// escribir 8 bytes
    Wire.endTransmission ();		// se termina la transmisión
    delay (300);					// espera para que se actualice el valor de temp
    Wire.requestFrom(9,8);			// solicito del esclavo 9 el envío de un byte
    if (Wire.available()){			// si el dato esta disponible en el BUS
		for (int i = 0; i < 8; i++) {		// leer 8 bytes
			msg[i]= Wire.read();			// que corresponde con la cadena encriptada
		}
		String cadena_descifrada;			// almacena el texto plano descifrado
		cadena_descifrada="";
		des.tripleDecrypt(texto, msg, key);	// desencripta la cadena de byte msg en texto
		for (int i = 0; i < 3; i++) {
			cadena_descifrada += String((char)texto[i]);
		}
		datos="Temp=";
		datos+=cadena_descifrada;
    }
    updateTFT();
    analogWrite(GREEN_PIN, 255);	// encender el led verde
    analogWrite(RED_PIN, 0);   	// apagar el led rojo
  }	     
}

// function that updates the LCD screen
void updateLCD() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(datos);
}
