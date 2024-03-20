/*
  Sketch name: Temp_Control_TFT_-_RS485
  UNED Arduino IoT lab. January 2021
*/

#include <SPI.h>
#include <SD.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <Adafruit_seesaw.h>
#include <Adafruit_TFTShield18.h>
#include <Wire.h>
#include "AESLib.h"

Adafruit_TFTShield18 ss;			//definición de la pantalla

// TFT display and SD card will share the hardware SPI interface.
// Hardware SPI pins are specific to the Arduino board type and
// cannot be remapped to alternate pins.  For Arduino Uno Wifi Rev2,
// ICSP-4 = MOSI, ICSP-1 = MISO, ICSP-3 = SCK.
#define SD_CS    4    // Chip select line for SD card on Shield
#define TFT_CS  10    // Chip select line for TFT display on Shield
#define TFT_DC   8    // Data/command line for TFT on Shield
#define TFT_RST  -1   // Reset line for TFT is handled by seesaw!

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

#define redStatusPin 5        // Led rojo
#define greenStatusPin 6      // Led Verde
#define blueStatusPin 9       // Led azul


AESLib aesLib;

#define INPUT_BUFFER_LIMIT (128+1) //Buffer de entrada limitado a 129


unsigned char cleartext[INPUT_BUFFER_LIMIT] = {0}; // cadena de caracteres en texto plano
unsigned char ciphertext[2*INPUT_BUFFER_LIMIT] = {0}; // Cadena de caracteres en texto cifrado

// // Clave de cifrado AES
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
  Serial1.begin(115200);  //configuración de la frecuencia del puerto serie
  while (!Serial1) {
    ; 
  }
  
  Wire.begin();               
  
  pinMode(greenStatusPin, OUTPUT);    	// Led verde encendido significa que el puerto serie esta libre
  pinMode(blueStatusPin, OUTPUT);     	// Led azul no se usará de momento
  pinMode(redStatusPin, OUTPUT);      	// Led rojo encendido significa que el puerto serie esta ocupado
  digitalWrite(greenStatusPin, 0);    	// encender led verde
  digitalWrite(blueStatusPin, 255);   	// apagar led azul  
  digitalWrite(redStatusPin, 255);		// apagar led rojo

  aes_init();
  
  initializeTFT();
  tft.setTextSize(2);
}

// Inicializacion de vectores
byte enc_iv[16] =      { 0x79, 0x4E, 0x98, 0x21, 0xAE, 0xD8, 0xA6, 0xAA, 0xD7, 0x97, 0x44, 0x14, 0xAB, 0xDD, 0x9F, 0x2C };
byte enc_iv_to[16]   = { 0x79, 0x4E, 0x98, 0x21, 0xAE, 0xD8, 0xA6, 0xAA, 0xD7, 0x97, 0x44, 0x14, 0xAB, 0xDD, 0x9F, 0x2C };
byte enc_iv_from[16] = { 0x79, 0x4E, 0x98, 0x21, 0xAE, 0xD8, 0xA6, 0xAA, 0xD7, 0x97, 0x44, 0x14, 0xAB, 0xDD, 0x9F, 0x2C };

String datos;               // datos para mostrar por pantalla
int tamano;					// tamaño en bytes de la cadena recibida
String msg;					// mensaje a enviar

void loop() {
	delay(10000);
	datos = "";
	digitalWrite(greenStatusPin, 255);   		// apagar led verde
	digitalWrite(redStatusPin,0);       		// encender led rojo
	msg="T";									// cadena en texto plano a enviar
	msg.toCharArray(cleartext, msg.length()+1);	// pasar la cadena a una tabla
	uint16_t msgLen = sizeof(cleartext);			// Guardo el tamalo de la cadena
	memcpy(enc_iv, enc_iv_to, sizeof(enc_iv_to));
	uint16_t encLen = encrypt_to_ciphertext((char*)cleartext, msgLen, enc_iv);	////escriptamos el texto cleartext en ciphertext
	Serial1.write(ciphertext,int(encLen));		// envio el texto cifrado
	delay(50);
	digitalWrite(greenStatusPin, 0);    		// encender el led verde
	digitalWrite(redStatusPin, 255);   			// apagar el led rojo
	
	//Espero a que el puerto este diponible con el dato solicitado
    int counter=0;
	while((Serial1.available()<=0) and (counter<50)){
		datos="Esperando";
		updateTFT();
      	counter=counter+1;
	}
	
	if (Serial1.available()){ 				 // si hay algún dato disponible
		digitalWrite(greenStatusPin, 255);  // apagar led verde
		digitalWrite(redStatusPin,0);      	// encender led rojo
		msg=Serial1.readString();			// leo la cadena del puerto serie
		msg.toCharArray(ciphertext, msg.length()+1);		// paso la cadena a una tabla
		memcpy(enc_iv, enc_iv_from, sizeof(enc_iv_from));
		uint16_t decLen = decrypt_to_cleartext(ciphertext, encLen , enc_iv);	//desciframos el texto del array ciphertext en cleartext
		datos="Temp=";
		datos+=(char*)cleartext;
		updateTFT();
		digitalWrite(greenStatusPin, 0);    // encender el led verde
		digitalWrite(redStatusPin, 255);   	// apagar el led rojo
	}     
}

// function that updates sensor readings and fan status in TFT screen
void updateTFT() {
  
  tft.fillScreen(ST77XX_BLACK);       //reset screen to black for new reading

  // Sensor readings:
  
  tft.setTextColor(ST77XX_YELLOW);
  tft.setCursor(0, 10);
  tft.setTextSize(4);
  tft.print(datos);
  
}

