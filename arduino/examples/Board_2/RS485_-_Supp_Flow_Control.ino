#include <LiquidCrystal.h>      // include LCD library

// Protocol constants
#define START_CHAR 'I'
#define TEMP_CMD 'T'
#define END_CHAR 'F'

// RS485 pins
#define TX_PIN 7                  
#define RX_PIN 8  

#define DELAY 20000


const int rs = 3, en = 4, d4 = 5, d5 = 6, d6 = 9, d7 = 10;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int temperature = 0;               // integer to hold temperature reading
long previousMillis = 0;

void setup() {
    Serial1.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);  // initialize serial port
    Serial1.setTimeout(1000);
    while (!Serial1) {
        ; // wait for serial port connection
    }

    lcd.begin(16, 2);           // set up the LCD's number of columns and rows
    lcd.clear();
}


void loop() {     
	long currentMillis = millis();   

	if(currentMillis-previousMillis >= DELAY){  
        previousMillis = currentMillis;
        Serial1.write(START_CHAR);
		Serial1.write(TEMP_CMD);
		Serial1.write(END_CHAR);
		Serial1.flush();
		delay(500);
	} 

    if(Serial1.available()){
        if (Serial1.find(START_CHAR)){
            temperature = Serial1.parseInt();
            updateLCD();
        }
        // clear the buffer
        while(Serial1.available()){
            Serial1.read();
        }
    }
}

// function that updates the LCD screen
void updateLCD() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print(" C");
}