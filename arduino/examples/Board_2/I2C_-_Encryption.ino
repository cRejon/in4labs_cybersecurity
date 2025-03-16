#include <LiquidCrystal.h>      // include LCD library
#include <Wire.h>           // include Wire library for I2C communication
#include <DES.h>            // include DES library for decryption


int temperature;               // integer to hold temperature reading
long previousMillis = 0;

const int rs = 3, en = 4, d4 = 5, d5 = 6, d6 = 9, d7 = 10;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

DES des;                    // DES object for encryption/decryption
byte key[] = { 
    0x3b, 0x38, 0x98, 0x37, 0x15, 0x20, 0xf7, 0x5e, // Key A
    0x92, 0x2f, 0xb5, 0x10, 0xc7, 0x1f, 0x43, 0x6e, // Key B
    0x3b, 0x38, 0x98, 0x37, 0x15, 0x20, 0xf7, 0x5e  // Key C
};                          // Define 3DES 64-bit key per row

void setup() {
  des.setKey(key);               // Initialize DES with the key
  
  Wire.begin();
  Wire.setWireTimeout(3000, true);

  lcd.begin(16, 2);           // set up the LCD's number of columns and rows
  lcd.clear(); 
}

void loop() {
  long currentMillis = millis();              
  
  if(currentMillis - previousMillis >= 20000){
    previousMillis = currentMillis;
    
    byte plainCmd[8] = {0};
    plainCmd[0] = 'T';
    byte cipherCmd[8];
    // your code here

    
    
    Wire.requestFrom(9, 8); // request 8-byte encrypted data
    delay(500);
    if(Wire.available() >= 8){
      byte cipherData[8];
      byte plainData[8];
      // your code here


      updateLCD();
    }
  }
}

void updateLCD() {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Temp: ");
    lcd.print(temperature);
    lcd.print(" C");
}