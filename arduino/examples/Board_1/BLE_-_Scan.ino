#include <ArduinoBLE.h>
#include <LiquidCrystal.h>      // include LCD library

#define RED_PIN 5  
#define GREEN_PIN 6
#define BLUE_PIN 9

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 17, en = 16, d4 = 15, d5 = 14, d6 = 4, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

String datos;
BLEDevice peripheral; 

void setup() {
  pinMode(GREEN_PIN, OUTPUT);    	// Led verde encendido significa que la placa de enciende
  pinMode(BLUE_PIN, OUTPUT);     	// Led azul encendido significará que esta buscando UUID
  pinMode(RED_PIN, OUTPUT);      	// Led rojo encendido significa que ha encontrado un UUID
  digitalWrite(GREEN_PIN, 255);    	// encender led verde
  digitalWrite(BLUE_PIN, 0);   	// apagar led azul  
  digitalWrite(RED_PIN, 0);		// apagar led rojo

  
  if (!BLE.begin()) {                   // Inicialización de BLE
    while (1);
  }
}

char dir_uuid[4];			//almacencamiento de la dirección BLE a buscar

void loop() {
  
}

// function that updates the LCD screen
void updateLCD() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(datos);
}
