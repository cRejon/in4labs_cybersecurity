#include <LiquidCrystal.h>      // include LCD library

#define RED_PIN A0  
#define GREEN_PIN A1
#define BLUE_PIN A2


const int rs = 3, en = 4, d4 = 5, d5 = 6, d6 = 9, d7 = 10;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int temperature;               // integer to hold temperature reading

void setup() {
    Serial0.begin(9600);  // set serial port baud rate
    while (!Serial0) {
        ; // wait for serial port connection
    }
    
    pinMode(GREEN_PIN, OUTPUT);    // Green LED on indicates the serial port is free
    pinMode(BLUE_PIN, OUTPUT);     // Blue LED is not used currently
    pinMode(RED_PIN, OUTPUT);       // Red LED on indicates the serial port is busy
    analogWrite(GREEN_PIN, 255);    // turn on green LED
    analogWrite(BLUE_PIN, 0);       // turn off blue LED  
    analogWrite(RED_PIN, 0);        // turn off red LED

    lcd.begin(16, 2);              // set up the LCD's number of columns and rows
    lcd.clear(); 
}

void loop() {
    analogWrite(GREEN_PIN, 0);       // turn off green LED
    analogWrite(RED_PIN, 255);       // turn on red LED
    Serial0.write("T");
    delay(200);
    analogWrite(GREEN_PIN, 255);     // turn on green LED
    analogWrite(RED_PIN, 0);         // turn off red LED
    delay(200);
    analogWrite(GREEN_PIN, 0);       // turn off green LED
    analogWrite(RED_PIN, 255);       // turn on red LED

    // Read one byte as the temperature
    if (Serial0.available()) {
        temperature = (int)Serial0.read();
        Serial0.flush();
        updateLCD();
    }
    
    analogWrite(GREEN_PIN, 255);     // turn on green LED
    analogWrite(RED_PIN, 0);         // turn off red LED
    
    delay(10000);
}

// Update updateLCD() to print the temperature integer
void updateLCD() {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Temp: ");
    lcd.print(temperature);
    lcd.print(" C");
}
