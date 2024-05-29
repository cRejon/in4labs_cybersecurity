#include <WiFi.h>   // Declaro la libreria WiFi
#include <LiquidCrystal.h>      // include LCD library

#define RED_PIN A0  
#define GREEN_PIN A1
#define BLUE_PIN A2

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 3, en = 4, d4 = 5, d5 = 6, d6 = 9, d7 = 10;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

char ssid[] = "WiFi-Access-Point";  // SSID a conectar
#define tam_clave 7
char* pwd[tam_clave]={"admin","adminttd","1234","admin1234","123456789","123456","admin123456789"};	//defino listado de contraseñas

void setup() {  
  Wire.begin();               
		
  WiFi.config(IPAddress(192, 168, 4, 3));	//Defino Ip del cliente
  
  pinMode(GREEN_PIN, OUTPUT);      // Se define el pin del led verde como salida
  pinMode(BLUE_PIN, OUTPUT);       // Se define el pin del led azul como salida
  pinMode(RED_PIN, OUTPUT);        // Se define el pin del led rojo como salida
  analogWrite(GREEN_PIN, 255);      // Se enciende el led verde
  analogWrite(BLUE_PIN, 0);     // Se apaga el led azul
  analogWrite(RED_PIN, 0);		// Se apaga el led rojo

  lcd.begin(16, 2);           // set up the LCD's number of columns and rows
  lcd.clear(); 
}

String datos;
int status = WL_IDLE_STATUS;

void loop() {
  
}

// function that updates the LCD screen
void updateLCD() {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print(datos);
}
