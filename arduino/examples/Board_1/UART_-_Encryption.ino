#include <DHT.h>  					//inclusión de la libreria para el sensor de temperatura
#include "AESLib.h"

#define RED_PIN A0  
#define GREEN_PIN A1
#define BLUE_PIN A2

#define DATA_PIN 2                   // define the type data pin
#define DHT_TYPE DHT22               // define the DHT sensor (DHT11, DHT21, or DHT22)

#define INPUT_BUFFER_LIMIT (128+1) //Buffer de entrada limitado a 129

DHT dht = DHT(DATA_PIN, DHT_TYPE);   // instantiate the dht class with our data pin and DHT type.

AESLib aesLib;

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
	Serial0.begin(115200);          	//configuración de la frecuencia del puerto serie
	while (!Serial0) {
		;
	}

	dht.begin();                

	pinMode(GREEN_PIN, OUTPUT);    // Led verde encendido significa que el puerto serie esta libre
	pinMode(BLUE_PIN, OUTPUT);     // Led azul no se usará de momento
	pinMode(RED_PIN, OUTPUT);      // Led rojo encendido significa que el puerto serie esta ocupado
	analogWrite(GREEN_PIN, 255);    // encender led verde
	analogWrite(BLUE_PIN, 0);   // apagar led azul  
	analogWrite(RED_PIN, 0);	// apagar led rojo

    aes_init();
}

String temperatureRead;     // variable para guardar la lectura del sensor
String msg;					//cadena leida del puerto serial
// Inicializacion de vectores
byte enc_iv[16] =      { 0x79, 0x4E, 0x98, 0x21, 0xAE, 0xD8, 0xA6, 0xAA, 0xD7, 0x97, 0x44, 0x14, 0xAB, 0xDD, 0x9F, 0x2C };
byte enc_iv_to[16]   = { 0x79, 0x4E, 0x98, 0x21, 0xAE, 0xD8, 0xA6, 0xAA, 0xD7, 0x97, 0x44, 0x14, 0xAB, 0xDD, 0x9F, 0x2C };
byte enc_iv_from[16] = { 0x79, 0x4E, 0x98, 0x21, 0xAE, 0xD8, 0xA6, 0xAA, 0xD7, 0x97, 0x44, 0x14, 0xAB, 0xDD, 0x9F, 0x2C };



void loop() {
	
	if (Serial0.available()){
		delay(50);
		msg=Serial0.readString();							//leo todo el texto del puerto serie a una cadena
		msg.toCharArray(ciphertext, msg.length()+1);		//paso el String a una tabla
		memcpy(enc_iv, enc_iv_from, sizeof(enc_iv_from));	//copiamos el array enc_iv_from al array enc_iv
		uint16_t decLen = decrypt_to_cleartext(ciphertext, msg.length() , enc_iv); //desciframos el texto del array ciphertext en cleartext
		if (String((char*)cleartext)=="T") {				// Si el valor recibido es una T
			temperatureRead = dht.readTemperature();  		// leer temperatura
			analogWrite(GREEN_PIN, 0);   			// apagar led verde
			analogWrite(RED_PIN, 255);       			// encender led rojo
			msg=String(temperatureRead.toInt());			// paso a cadena la temperatura
			msg.toCharArray(cleartext, msg.length()+1);		// paso la cadena (String) a una tabla
			uint16_t msgLen = sizeof(cleartext);			// almaceno el tamaño del texto cifrado
			memcpy(enc_iv, enc_iv_to, sizeof(enc_iv_to));
			uint16_t encLen = encrypt_to_ciphertext((char*)cleartext, msgLen, enc_iv);	//escriptamos el texto cleartext en ciphertext
			Serial0.write(ciphertext,int(encLen));			// enviamos el texto cifrado
			delay(100);
			analogWrite(GREEN_PIN, 255);    			// encender el led verde
			analogWrite(RED_PIN, 0);   				// apagar el led rojo 
        }
	}
}
