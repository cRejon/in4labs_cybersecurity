#include <Wire.h> 			// incluye la libreria Wire
#include <LiquidCrystal.h>      // include LCD library

#define RED_PIN 5  
#define GREEN_PIN 6
#define BLUE_PIN 9

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 17, en = 16, d4 = 15, d5 = 14, d6 = 4, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

char tempRead[5];               // char array to hold temperature reading

void setup() {
  Wire.begin();
  pinMode(GREEN_PIN, OUTPUT);    	// Led verde encendido significa que el puerto serie esta libre
  pinMode(BLUE_PIN, OUTPUT);     	// Led azul no se usará de momento
  pinMode(RED_PIN, OUTPUT);      	// Led rojo encendido significa que el puerto serie esta ocupado
  digitalWrite(GREEN_PIN, 255);    	// encender led verde
  digitalWrite(BLUE_PIN, 0);   	// apagar led azul  
  digitalWrite(RED_PIN, 0);	// apagar led rojo

  lcd.begin(16, 2);           // set up the LCD's number of columns and rows
  lcd.clear(); 
}

String tempRead;

void loop() {
	   
}

// function that updates the LCD screen
void updateLCD() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Temp:");
  lcd.print(tempRead);
  lcd.print(" C");
}
