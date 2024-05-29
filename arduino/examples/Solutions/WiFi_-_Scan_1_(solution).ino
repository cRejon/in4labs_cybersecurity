#include <WiFi.h>   // Declaro la libreria WiFi
#include <LiquidCrystal.h>      // include LCD library

#define RED_PIN A0  
#define GREEN_PIN A1
#define BLUE_PIN A2

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 3, en = 4, d4 = 5, d5 = 6, d6 = 9, d7 = 10;
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
    datos="Buscando redes";
    updateLCD();						//muestra mensaje mientras busca
    delay(2000);
    int n_SSID=WiFi.scanNetworks();	//escanea en busca de redes 
    if (n_SSID!=-1) {					//si ha encontrado algo
        datos="Num Redes=";
        datos+=n_SSID;
        updateLCD();
    }
    delay(20000);                     //espera de 20 segundos  
}

// function that updates the LCD screen
void updateLCD() {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print(datos);
}
