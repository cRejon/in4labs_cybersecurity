#include <LiquidCrystal.h>      // include LCD library
#include "AESLib.h"

#define RED_PIN A0  
#define GREEN_PIN A1
#define BLUE_PIN A2

#define INPUT_BUFFER_LIMIT (128+1) //Buffer de entrada limitado a 129

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 3, en = 4, d4 = 5, d5 = 6, d6 = 9, d7 = 10;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

AESLib aesLib;

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
  Serial0.begin(115200);  //configuración de la frecuencia del puerto serie
  while (!Serial0) {
    ; 
  }          
  
  pinMode(GREEN_PIN, OUTPUT);    	// Led verde encendido significa que el puerto serie esta libre
  pinMode(BLUE_PIN, OUTPUT);     	// Led azul no se usará de momento
  pinMode(RED_PIN, OUTPUT);      	// Led rojo encendido significa que el puerto serie esta ocupado
  analogWrite(GREEN_PIN, 255);    	// encender led verde
  analogWrite(BLUE_PIN, 0);   	// apagar led azul  
  analogWrite(RED_PIN, 0);		// apagar led rojo

  aes_init();
  
  lcd.begin(16, 2);           // set up the LCD's number of columns and rows
  lcd.clear(); 
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
	analogWrite(GREEN_PIN, 0);   		// apagar led verde
	analogWrite(RED_PIN, 255);       		// encender led rojo
	msg="T";									// cadena en texto plano a enviar
	msg.toCharArray(cleartext, msg.length()+1);	// pasar la cadena a una tabla
	uint16_t msgLen = sizeof(cleartext);			// Guardo el tamalo de la cadena
	memcpy(enc_iv, enc_iv_to, sizeof(enc_iv_to));
	uint16_t encLen = encrypt_to_ciphertext((char*)cleartext, msgLen, enc_iv);	////escriptamos el texto cleartext en ciphertext
	Serial0.write(ciphertext,int(encLen));		// envio el texto cifrado
	delay(50);
	analogWrite(GREEN_PIN, 255);    		// encender el led verde
	analogWrite(RED_PIN, 0);   			// apagar el led rojo
	
	//Espero a que el puerto este diponible con el dato solicitado
    int counter=0;
	while((Serial0.available()<=0) and (counter<50)){
		datos="Esperando";
		updateLCD();
      	counter=counter+1;
	}
	
	if (Serial0.available()){ 				 // si hay algún dato disponible
		analogWrite(GREEN_PIN, 0);  // apagar led verde
		analogWrite(RED_PIN, 255);      	// encender led rojo
		msg=Serial0.readString();			// leo la cadena del puerto serie
		msg.toCharArray(ciphertext, msg.length()+1);		// paso la cadena a una tabla
		memcpy(enc_iv, enc_iv_from, sizeof(enc_iv_from));
		uint16_t decLen = decrypt_to_cleartext(ciphertext, encLen , enc_iv);	//desciframos el texto del array ciphertext en cleartext
		datos="Temp=";
		datos+=(char*)cleartext;
		updateLCD();
		analogWrite(GREEN_PIN, 255);    // encender el led verde
		analogWrite(RED_PIN, 0);   	// apagar el led rojo
	}     
}

// function that updates the LCD screen
void updateLCD() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Temp:");
  lcd.print(tempRead);
  lcd.print(" C");
}

