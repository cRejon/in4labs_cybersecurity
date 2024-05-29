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
int temperature;            // var almacena temperatura
long previousMillis = 0;

void loop() {
	datos = "";
	temperature= 0;       
	long currentMillis = millis();              
	if(currentMillis-previousMillis>= 20000){    	// Si la temperatura se ha leido hace mas de 20seg
		previousMillis=currentMillis;
		analogWrite(GREEN_PIN, 0);   		// apagar led verde
		analogWrite(RED_PIN, 255);       		// encender led rojo
		delay(100);
		Serial1.write("I");
		Serial1.write("T");
		Serial1.write("F");
		Serial1.flush();
		delay(200);
		analogWrite(GREEN_PIN, 255);    		// encender el led verde
		analogWrite(RED_PIN, 0);   			// apagar el led rojo 
		if (find_c('I',10000)){
			analogWrite(GREEN_PIN, 0);  	// apagar led verde
			analogWrite(RED_PIN, 255);       	// encender led rojo
			temperature=int(Serial1.read());
			datos="Temp=";
			datos+=String(temperature);
			if (Serial1.read()=='F') {				// fin de trama recibido
				if (temperature > temperatureLimit){
					datos+=" Encender ventilador";
				}
				else {
					datos+=" Apagar ventilador";
				}
				updateLCD();                        // actualiza la pantalla TFT
			}
			analogWrite(GREEN_PIN, 255);    	// encender el led verde
			analogWrite(RED_PIN, 0);   		// apagar el led rojo	
		}
	} 
}

//Implementación de función FIND de Serial
boolean find_c(char l, unsigned long duration)
{
	//implementacion de serial.find
	boolean encontrado = false;
	unsigned long currentTime;

	currentTime = millis();
	while(Serial1.available()<=0){
		if (millis() - currentTime > duration) {
			return encontrado;
		}
		datos="Esperando";
		updateLCD();
	}
	char c=Serial1.read();
	while ((c!=l)&&(Serial1.available()>0)){
		c=Serial1.read();
		datos="Esperando I";
		updateLCD();
		delay(100);
		if (millis() - currentTime > duration) {
			return encontrado;
		}
		else if (c==l) {
			encontrado=true;
		}
	}
	return encontrado;
}

// function that updates the LCD screen
void updateLCD() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Temp:");
  lcd.print(temperature);
  lcd.print(" C");
}