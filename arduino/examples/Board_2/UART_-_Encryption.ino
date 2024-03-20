
#include <DHT.h>  					//inclusión de la libreria para el sensor de temperatura
#include <Wire.h>
#include "AESLib.h"

#define dataPin 10                  // pin de conexión con el sensor de temperatura
#define DHTType DHT22               // define tipo de sensor

DHT dht = DHT(dataPin, DHTType);    // instancia el sensor con el puerto

#define redStatusPin 5        		// led rojo
#define greenStatusPin 6      		// led verde
#define blueStatusPin 9       		// led azul

AESLib aesLib;

#define INPUT_BUFFER_LIMIT (128+1) //Buffer de entrada limitado a 129


unsigned char cleartext[INPUT_BUFFER_LIMIT] = {0}; // cadena de caracteres en texto plano
unsigned char ciphertext[2*INPUT_BUFFER_LIMIT] = {0}; // Cadena de caracteres en texto cifrado

// Clave de cifrado AES
byte aes_key[] = {  57, 36, 24, 25, 28, 86, 32, 41, 31, 36, 91, 36, 51, 74, 63, 89 };

// Vector de inicialización general
byte aes_iv[16] = { 0x79, 0x4E, 0x98, 0x21, 0xAE, 0xD8, 0xA6, 0xAA, 0xD7, 0x97, 0x44, 0x14, 0xAB, 0xDD, 0x9F, 0x2C };

// Función para generar bloques aleatorio
void aes_init() {
  aesLib.gen_iv(aes_iv);
  aesLib.set_paddingmode((paddingMode)0);
}

uint16_t encrypt_to_ciphertext(char * msg, uint16_t msgLen, byte iv[]) {
  int cipherlength = aesLib.encrypt((byte*)msg, msgLen, (char*)ciphertext, aes_key, sizeof(aes_key), iv);
  return cipherlength;
}

uint16_t decrypt_to_cleartext(byte msg[], uint16_t msgLen, byte iv[]) {
  uint16_t dec_bytes = aesLib.decrypt(msg, msgLen, (char*)cleartext, aes_key, sizeof(aes_key), iv);
  return dec_bytes;
}

void setup() {
	Serial1.begin(115200);          	//configuración de la frecuencia del puerto serie
	while (!Serial1) {
		;
	}

	dht.begin();                

	pinMode(greenStatusPin, OUTPUT);    // Led verde encendido significa que el puerto serie esta libre
	pinMode(blueStatusPin, OUTPUT);     // Led azul no se usará de momento
	pinMode(redStatusPin, OUTPUT);      // Led rojo encendido significa que el puerto serie esta ocupado
	digitalWrite(greenStatusPin, 0);    // encender led verde
	digitalWrite(blueStatusPin, 255);   // apagar led azul  
	digitalWrite(redStatusPin, 255);	// apagar led rojo

    aes_init();

	Wire.begin(9);
}

String temperatureRead;     // variable para guardar la lectura del sensor
String msg;					//cadena leida del puerto serial
// Inicializacion de vectores
byte enc_iv[16] =      { 0x79, 0x4E, 0x98, 0x21, 0xAE, 0xD8, 0xA6, 0xAA, 0xD7, 0x97, 0x44, 0x14, 0xAB, 0xDD, 0x9F, 0x2C };
byte enc_iv_to[16]   = { 0x79, 0x4E, 0x98, 0x21, 0xAE, 0xD8, 0xA6, 0xAA, 0xD7, 0x97, 0x44, 0x14, 0xAB, 0xDD, 0x9F, 0x2C };
byte enc_iv_from[16] = { 0x79, 0x4E, 0x98, 0x21, 0xAE, 0xD8, 0xA6, 0xAA, 0xD7, 0x97, 0x44, 0x14, 0xAB, 0xDD, 0x9F, 0x2C };



void loop() {
	
	if (Serial1.available()){
		delay(50);
		msg=Serial1.readString();							//leo todo el texto del puerto serie a una cadena
		msg.toCharArray(ciphertext, msg.length()+1);		//paso el String a una tabla
		memcpy(enc_iv, enc_iv_from, sizeof(enc_iv_from));	//copiamos el array enc_iv_from al array enc_iv
		uint16_t decLen = decrypt_to_cleartext(ciphertext, msg.length() , enc_iv); //desciframos el texto del array ciphertext en cleartext
		if (String((char*)cleartext)=="T") {				// Si el valor recibido es una T
			temperatureRead = dht.readTemperature();  		// leer temperatura
			digitalWrite(greenStatusPin, 255);   			// apagar led verde
			digitalWrite(redStatusPin,0);       			// encender led rojo
			msg=String(temperatureRead.toInt());			// paso a cadena la temperatura
			msg.toCharArray(cleartext, msg.length()+1);		// paso la cadena (String) a una tabla
			uint16_t msgLen = sizeof(cleartext);			// almaceno el tamaño del texto cifrado
			memcpy(enc_iv, enc_iv_to, sizeof(enc_iv_to));
			uint16_t encLen = encrypt_to_ciphertext((char*)cleartext, msgLen, enc_iv);	//escriptamos el texto cleartext en ciphertext
			Serial1.write(ciphertext,int(encLen));			// enviamos el texto cifrado
			delay(100);
			digitalWrite(greenStatusPin, 0);    			// encender el led verde
			digitalWrite(redStatusPin, 255);   				// apagar el led rojo 
        }
	}
}
