#include <Wire.h> 			    // Include Wire library for I2C communication
#include <LiquidCrystal.h>      // include LCD library


// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 3, en = 4, d4 = 5, d5 = 6, d6 = 9, d7 = 10;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  Wire.begin();

  lcd.begin(16, 2);           // set up the LCD's number of columns and rows
  lcd.clear(); 
}

String data;

void loop() {
	int numDevices=0;
	byte error;
	byte i2cAddress;

	data="**Scaning**";
	updateLCD();
	delay(1000);

	for(i2cAddress = 1; i2cAddress < 127; i2cAddress++){
       	Wire.beginTransmission(i2cAddress);
       	error = Wire.endTransmission();
     
       	if (error == 0){
			numDevices++;
       		data = "Device dir: ";
           	data += i2cAddress;

			updateLCD();
       		delay(5000);
       	}
    }

    data="Total dev: ";
    data += numDevices;
    updateLCD();

  	delay(20000);    
}

// function that updates the LCD screen
void updateLCD() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(data);
}
