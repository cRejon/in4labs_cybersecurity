#include <SoftwareSerial.h>     // include SoftwareSerial library
#include <LiquidCrystal.h>      // include LCD library

//SoftwareSerial pins
#define RX_PIN 7                       // connected to RO (Receiver Out)
#define TX_PIN 8                       // connected to DI (Driver In)
#define TRANSMIT_ENABLER 3             // high to send data. low to receive

#define RED_PIN 5  
#define GREEN_PIN 6
#define BLUE_PIN 9

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 17, en = 16, d4 = 15, d5 = 14, d6 = 4, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

SoftwareSerial RS485 (RX_PIN, TX_PIN); // assign name, RX and TX pins to software serial

String temperatureRead;               // make a String to hold data received from the sensor

void setup() {
  RS485.begin(9600);  //configuración de la frecuencia del puerto serie
  while (!RS485) {
    ; 
  }           
  
  pinMode(GREEN_PIN, OUTPUT);    	// Led verde encendido significa que el puerto serie esta libre
  pinMode(BLUE_PIN, OUTPUT);     	// Led azul no se usará de momento
  pinMode(RED_PIN, OUTPUT);      	// Led rojo encendido significa que el puerto serie esta ocupado
  analogWrite(GREEN_PIN, 255);    	// encender led verde
  analogWrite(BLUE_PIN, 0);   	// apagar led azul  
  analogWrite(RED_PIN, 0);		// apagar led rojo

  pinMode(TRANSMIT_ENABLER, OUTPUT);      // output signal to control MAX485 mode
  digitalWrite(TRANSMIT_ENABLER, LOW);    // setup as slave. MAX485 receiving data

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
		digitalWrite(TRANSMIT_ENABLER, HIGH); 		// habilitar el puerto serie para transmisión
		digitalWrite(GREEN_PIN, 0);   		// apagar led verde
		digitalWrite(RED_PIN, 255);       		// encender led rojo
		delay(100);
		RS485.write("I");
		RS485.write("T");
		RS485.write("F");
		RS485.flush();
		digitalWrite(TRANSMIT_ENABLER, LOW); 			// deshabilita el puerto serie para transmisión
		delay(200);
		digitalWrite(GREEN_PIN, 255);    		// encender el led verde
		digitalWrite(RED_PIN, 0);   			// apagar el led rojo 
		if (find_c('I',10000)){
			digitalWrite(GREEN_PIN, 0);  	// apagar led verde
			digitalWrite(RED_PIN, 255);       	// encender led rojo
			temperature=int(RS485.read());
			datos="Temp=";
			datos+=String(temperature);
			if (RS485.read()=='F') {				// fin de trama recibido
				if (temperature > temperatureLimit){
					datos+=" Encender ventilador";
				}
				else {
					datos+=" Apagar ventilador";
				}
				updateLCD();                        // actualiza la pantalla TFT
			}
			digitalWrite(GREEN_PIN, 255);    	// encender el led verde
			digitalWrite(RED_PIN, 0);   		// apagar el led rojo	
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
	while(RS485.available()<=0){
		if (millis() - currentTime > duration) {
			return encontrado;
		}
		datos="Esperando";
		updateTFT();
	}
	char c=RS485.read();
	while ((c!=l)&&(RS485.available()>0)){
		c=RS485.read();
		datos="Esperando I";
		updateTFT();
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
  lcd.print(tempRead);
  lcd.print(" C");
}