#include <LiquidCrystal.h>      // include LCD library
#include <Wire.h>           // include Wire library for I2C communication
#include <DES.h>            // include DES library for decryption

#define DELAY 4000        // Delay between temperature readings

const int rs = 3, en = 4, d4 = 5, d5 = 6, d6 = 9, d7 = 10;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Define encryption key (must match the key in the other Arduino)
byte encryptionKey[24] = {
  0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
  0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10,
  0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88
};

// Buffers for decryption
byte encryptedData[8] = {0};
byte decryptedData[8] = {0};

DES des;                   // Create DES object for decryption

void setup() {
    Wire.begin();          // Set up I2C communication

    des.init(encryptionKey);  // Initialize DES with our key

    lcd.begin(16, 2);              // set up the LCD's number of columns and rows
    lcd.clear(); 
}

int temperature;           // Variable to store temperature
long previousMillis = 0;

void loop() {
    long currentMillis = millis();              
    
    if (currentMillis-previousMillis >= DELAY){    // Check temperature every 20 seconds
        previousMillis=currentMillis;
        
        Wire.beginTransmission(9);     // Start transmission to slave address 9
        Wire.write('T');               // Send temperature request command
        Wire.endTransmission();        // End transmission
        delay(1000);                    // Wait for temperature update
        
        // Request 8 bytes (encrypted data) from slave device
        Wire.requestFrom(9, 8);
        
        // Read all 8 bytes into our buffer
        if (Wire.available() >= 8) {
            for (int i = 0; i < 8; i++) {
                encryptedData[i] = Wire.read();
            }
            
            // Decrypt the data
            des.tripleDecrypt(decryptedData, encryptedData, encryptionKey);
            
            // Extract the temperature from the first byte
            temperature = (int)decryptedData[0];
            
            updateLCD();
        }
    }
}

// Update updateLCD() to print the temperature integer
void updateLCD() {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Temp: ");
    lcd.print(temperature);
    lcd.print(" C");
}