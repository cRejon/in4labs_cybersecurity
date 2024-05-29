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
	int esclavos=0;
	byte error;
	byte direccion;
	datos="**Escaneando**";
	updateLCD();
	delay(1000);
	for(direccion = 1; direccion < 127; direccion++ )
    {
       	Wire.beginTransmission(direccion);
       	error = Wire.endTransmission();
     
       	if (error == 0)
       	{
       		datos="Esclavo en dir ";
           	datos+=direccion;
			updateLCD();
			esclavos++;
       		delay(5000);
       	}
    }
    if (esclavos == 0)
	{
       	datos="No hay dispositivos";
		updateLCD();
	}
    else
	{	
       	datos="Se han encontrado ";
       	datos+=esclavos;
       	datos+=" dispositivos.";
		updateLCD();
    }
  	delay(30000);     	//espera 30 seg para el siguiente escaner     
}

// function that updates the LCD screen
void updateLCD() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(datos);
}
