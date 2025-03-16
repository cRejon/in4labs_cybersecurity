#include <LiquidCrystal.h>      // include LCD library
#include "AESLib.h"             // include AES library for decryption

#define DELAY 10000             // define the delay between temperature readings

// AES Encryption Key (must be same on both devices)
byte aes_key[] = { 0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C };

// Initialization vector 
byte aes_iv[N_BLOCK] = { 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA };

AESLib aesLib;
unsigned char ciphertext[32]; // buffer for received encrypted data
unsigned char cleartext[16]; // buffer for decrypted data

int temperature;               // integer to hold temperature reading

const int rs = 3, en = 4, d4 = 5, d5 = 6, d6 = 9, d7 = 10;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Function to decrypt received data
uint16_t decryptData(byte msg[], uint16_t msgLen, byte iv[]) {
  uint16_t dec_bytes = aesLib.decrypt(msg, msgLen, cleartext, aes_key, sizeof(aes_key), iv);
  cleartext[dec_bytes] = '\0'; // Ensure null termination
  return dec_bytes;
}

void setup() {
    Serial0.begin(9600);  // set serial port baud rate
    while (!Serial0) {
        ; // wait for serial port connection
    }

    lcd.begin(16, 2);              // set up the LCD's number of columns and rows
    lcd.clear();
    
    aesLib.gen_iv(aes_iv);
    aesLib.set_paddingmode((paddingMode)0);
}

void loop() {
    Serial0.write('T');
    delay(200);

    // First read the length of encrypted data
    if (Serial0.available()) {
        uint8_t encLen = Serial0.read();
        delay(100); // Wait for data to arrive
        
        // Read the encrypted data
        int bytesRead = 0;
        while(Serial0.available() && bytesRead < encLen) {
            ciphertext[bytesRead++] = Serial0.read();
        }
        
        if (bytesRead == encLen) {
            // Make a local copy of IV to use for decryption
            byte dec_iv[N_BLOCK];
            memcpy(dec_iv, aes_iv, sizeof(aes_iv));
            
            // Decrypt the data
            uint16_t decLen = decryptData(ciphertext, encLen, dec_iv);
            
            // Convert decrypted string to integer
            temperature = atoi((char*)cleartext);
            
            // Update the LCD
            updateLCD();
        }
        
        Serial0.flush();
    }
    
    delay(DELAY);
}

// Update updateLCD() to print the temperature integer
void updateLCD() {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Temp: ");
    lcd.print(temperature);
    lcd.print(" C");
}
