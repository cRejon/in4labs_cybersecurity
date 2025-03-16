#include <LiquidCrystal.h>      // include LCD library

// RS-485 pins
#define TX_PIN 7                  
#define RX_PIN 8  


int temperature = 0;               // integer to hold temperature reading

const int rs = 3, en = 4, d4 = 5, d5 = 6, d6 = 9, d7 = 10;  // LCD pins
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); 

void setup() {
    Serial1.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);  // initialize serial port
    while (!Serial1) {
        ; // wait for serial port connection
    }       

    lcd.begin(16, 2);           // set up the LCD's number of columns and rows
    lcd.clear();
}

void loop() {
    // Read one byte as the temperature
    if (Serial1.available()) {
        temperature = int(Serial1.read());
        updateLCD();
    }
    
    delay(500); 
}

// function that updates the LCD screen
void updateLCD() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print(" C");
}
