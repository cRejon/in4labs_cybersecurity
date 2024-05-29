#include <LiquidCrystal.h>      // include LCD library

#define RED_PIN A0  
#define GREEN_PIN A1
#define BLUE_PIN A2

//HardwareSerial pins
#define TX_PIN 7                  
#define RX_PIN 8  

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 3, en = 4, d4 = 5, d5 = 6, d6 = 9, d7 = 10;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

String temperatureRead;               // make a String to hold data received from the sensor

void setup() {
    Serial1.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);  // initialize serial port
    delay(1000);          // wait for serial port to initialize        
  
    pinMode(GREEN_PIN, OUTPUT);    	// Led verde encendido significa que el puerto serie esta libre
    pinMode(BLUE_PIN, OUTPUT);     	// Led azul no se usará de momento
    pinMode(RED_PIN, OUTPUT);      	// Led rojo encendido significa que el puerto serie esta ocupado
    analogWrite(GREEN_PIN, 255);    	// encender led verde
    analogWrite(BLUE_PIN, 0);   	// apagar led azul  
    analogWrite(RED_PIN, 0);		// apagar led rojo

    lcd.begin(16, 2);           // set up the LCD's number of columns and rows
    lcd.clear();
}

String datos;               // datos recibidor por el sensor

void loop() {
	datos = "";
	if (Serial1.available()){             	// hay datos para leer en el puerto serie
		analogWrite(GREEN_PIN, 0);  // apagar led verde
		analogWrite(RED_PIN, 255);      	// encender led rojo
		delay(100);
		char c = Serial1.read();
		datos="T=";
		datos+= String(int(c));
		if (int(c)>0){							//Si el dato recibido no es basura
			if (int(c)>40) {
				datos+=" Encender ventilador";
			}
			else {
				datos+=" Apagar ventilador";
			}
			analogWrite(GREEN_PIN, 255);    // encender el led verde
			analogWrite(RED_PIN, 0);   	// apagar el led rojo
			updateLCD();                        // actualiza la pantalla LCD
		}
		delay(2000);                     	// espera activa de 2 seg
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
