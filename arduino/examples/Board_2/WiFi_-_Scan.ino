#include <WiFi.h>   // Declaro la libreria WiFi
#include <LiquidCrystal.h>      // include LCD library

#define RED_PIN A0  
#define GREEN_PIN A1
#define BLUE_PIN A2

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 3, en = 4, d4 = 5, d5 = 6, d6 = 9, d7 = 10;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 17, en = 16, d4 = 15, d5 = 14, d6 = 4, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {  
    pinMode(GREEN_PIN, OUTPUT);      // Se define el pin del led verde como salida
    pinMode(BLUE_PIN, OUTPUT);       // Se define el pin del led azul como salida
    pinMode(RED_PIN, OUTPUT);        // Se define el pin del led rojo como salida
    analogWrite(GREEN_PIN, 255);      // Se enciende el led verde
    analogWrite(BLUE_PIN, 0);     // Se apaga el led azul
    analogWrite(RED_PIN, 0);    // Se apaga el led rojo

    lcd.begin(16, 2);           // set up the LCD's number of columns and rows
    lcd.clear(); 
}

String datos;
String SSID;               

void loop() {
 
}

// function that updates the LCD screen
void updateLCD() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(datos);
}
