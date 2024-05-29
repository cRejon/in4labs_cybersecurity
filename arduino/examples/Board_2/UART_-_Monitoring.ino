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
  Serial0.begin(115200);  //configuración de la frecuencia del puerto serie
  while (!Serial0) {
    ; 
  }          
  
  pinMode(GREEN_PIN, OUTPUT);    	// Led verde encendido significa que el puerto serie esta libre
  pinMode(BLUE_PIN, OUTPUT);     	// Led azul no se usará de momento
  pinMode(RED_PIN, OUTPUT);      	// Led rojo encendido significa que el puerto serie esta ocupado
  analogWrite(GREEN_PIN, 255);    	// encender led verde
  analogWrite(BLUE_PIN, 0);   	// apagar led azul  
  analogWrite(RED_PIN, 0);		// apagar led rojo

  lcd.begin(16, 2);           // set up the LCD's number of columns and rows
  lcd.clear(); 
}

int tempRead;						// caracter recibido por el sensor

void loop() {
	analogWrite(GREEN_PIN, 0);   		// apagar led verde
	analogWrite(RED_PIN, 255);       		// encender led rojo
	Serial0.write("T");
	delay(200);
	analogWrite(GREEN_PIN, 255);    		// encender el led verde
	analogWrite(RED_PIN, 0);   			// apagar el led rojo
	delay(200);
	analogWrite(GREEN_PIN, 0);  // apagar led verde
	analogWrite(RED_PIN, 255);      	// encender led rojo
    int i = 0;                          // counter for number of bytes received
    while(Serial0.available()){         // slave may send less than requested
        char c = Serial0.read();          // receive a byte as character
        tempRead[i] =  c;                 // load received char into char array
        i++;
        if (i == 5){
        break;  
        }

        updateLCD();
    }

	analogWrite(GREEN_PIN, 255);    // encender el led verde
	analogWrite(RED_PIN, 0);   	// apagar el led rojo
	
	delay(10000);
}

// function that updates the LCD screen
void updateLCD() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Temp:");
  lcd.print(tempRead);
  lcd.print(" C");
}
