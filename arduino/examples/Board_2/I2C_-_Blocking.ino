#include <Wire.h> 			// incluye la libreria Wire
#include <LiquidCrystal.h>      // include LCD library

#define RED_PIN A0  
#define GREEN_PIN A1
#define BLUE_PIN A2

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 3, en = 4, d4 = 5, d5 = 6, d6 = 9, d7 = 10;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

char tempRead[5];               // char array to hold temperature reading

void setup() {
  Wire.begin();
  Wire.setWireTimeout(3000, true);
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
int temperature;            // var almacena temperatura
long previousMillis = 0;

void loop() {
  temperature= 0;       
  long currentMillis = millis();              
  if(currentMillis-previousMillis>= 20000){    	// Si la temperatura se ha leido hace mas de 20seg
    previousMillis=currentMillis;
    analogWrite(GREEN_PIN, 0);  	// apagar led verde
    analogWrite(RED_PIN, 255);       	// encender led rojo	
    Wire.beginTransmission (9);		// empezar transmisión con el esclavo 9
    Wire.write("T");				// se envia un byte concretamente la letra T
    Wire.endTransmission ();		// se termina la transmisión
    delay (100);					// espera para que se actualice el valor de temp
    Wire.requestFrom(9,1);			// solicito del esclavo 9 el envío de un byte
    if (Wire.available()){			// si el dato esta disponible en el BUS
	char c=Wire.read();			// leo un byte
	temperature=int(c);
	datos="Temp=";
	datos+=temperature;
	if (temperature>25)             // si la temperatura es mayor de 25
		datos+= "**Encender Ventilador**";   
	else
		datos+= "**Apagar Ventilador**";   // apagar el ventilador
    }
    updateLCD();
    analogWrite(GREEN_PIN, 255);	// encender el led verde
    analogWrite(RED_PIN, 0);   	// apagar el led rojo
  }	     
}

// function that updates the LCD screen
void updateLCD() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(datos);
}