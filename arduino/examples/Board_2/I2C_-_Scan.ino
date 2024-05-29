#include <Wire.h> 			// incluye la libreria Wire
#include <LiquidCrystal.h>      // include LCD library

#define RED_PIN A0  
#define GREEN_PIN A1
#define BLUE_PIN A2

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 3, en = 4, d4 = 5, d5 = 6, d6 = 9, d7 = 10;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  Wire.begin();
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

void loop() {
	   
}

// function that updates the LCD screen
void updateLCD() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(datos);
}
