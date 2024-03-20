#include <ArduinoBLE.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>   // include I2C library (Used to avoid communication issues with TFT screen)

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <Adafruit_seesaw.h>
#include <Adafruit_TFTShield18.h>

Adafruit_TFTShield18 ss;


#define SD_CS    4    // Chip select line for SD card on Shield
#define TFT_CS  10    // Chip select line for TFT display on Shield
#define TFT_DC   8    // Data/command line for TFT on Shield
#define TFT_RST  -1   // Reset line for TFT is handled by seesaw!

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
#define redStatusPin 5        // Led rojo
#define greenStatusPin 6      // Led Verde
#define blueStatusPin 9       // Led azul

String datos;
BLEDevice peripheral; 

void setup() {
  Wire.begin();               // join i2c as slave bus with address #2 to avoid communication issues with TFT screen

  
    
  pinMode(greenStatusPin, OUTPUT);    	// Led verde encendido significa que la placa de enciende
  pinMode(blueStatusPin, OUTPUT);     	// Led azul encendido significará que esta buscando UUID
  pinMode(redStatusPin, OUTPUT);      	// Led rojo encendido significa que ha encontrado un UUID
  digitalWrite(greenStatusPin, 0);    	// encender led verde
  digitalWrite(blueStatusPin, 255);   	// apagar led azul  
  digitalWrite(redStatusPin, 255);	// apagar led rojo

  
  if (!BLE.begin()) {                   // Inicialización de BLE
    while (1);
  }
}

char dir_uuid[4]="0005";			//almacencamiento de la dirección BLE a buscar

void loop() {
  digitalWrite(greenStatusPin, 255);	// Apaga el LED VERDE
  digitalWrite(blueStatusPin, 0);   	// Enciende el led azul para indicar que esta buscando UUID  
  digitalWrite(redStatusPin, 255);		// Apaga el LED ROJO
  BLE.scanForUuid(dir_uuid);      		// Escanea dir
  peripheral = BLE.available(); 		// Devuelve verdadero si descubre algun dispositivo
  int counter = 0;              		// Establecer a 0 el numero de intentos de conexion
  while(!peripheral && counter < 10){  	// mientras no se decubra y no se llegue a 10 intentos
	BLE.scanForUuid(dir_uuid);          // busco el dispositivo
	delay(10);        
	peripheral = BLE.available();   	// Devuelve verdadero si descubre algun dispositivo        
	counter =counter+1;             	// Incrementar el contador
  }  
  BLE.stopScan();						// Parar el escaneo
  digitalWrite(greenStatusPin, 0);    	// Enciende el LED VERDE
  digitalWrite(blueStatusPin, 255);   	// Apaga el LED AZUL- ha parado la busqueda  
  digitalWrite(redStatusPin, 255);		// Apaga el LED ROJO
  if (peripheral) {						// Si ha encontrado un periférico
	if (peripheral.connect()) {  		// Se conecta al BLE
		if (peripheral.discoverService(dir_uuid)) {  //Se conecta al servicio	
			digitalWrite(greenStatusPin, 255);    	// Apaga el LED VERDE
			digitalWrite(blueStatusPin, 255);   	// Apaga el LED AZUL  
			digitalWrite(redStatusPin, 0);			// Enciende LED ROJO-Dispositivo encontrado
			datos="Hay Características:";
			for (int i = 0; i < peripheral.characteristicCount(); i++) {
				datos+=peripheral.characteristic(i).uuid();
				datos+=" ";
			}
			initializeTFT();
			delay(2000);
			updateTFT();
			delay(20000);
			digitalWrite(greenStatusPin, 0);    	// Enciende LED VERDE
			digitalWrite(blueStatusPin, 255);   	// Apaga el LED AZUL  
			digitalWrite(redStatusPin, 255);		// Apaga LED ROJO
		}
		peripheral.disconnect();
	}
  }
}

void updateTFT() {
 
  tft.fillScreen(ST77XX_BLACK);       //reset screen to black for new reading
  
  tft.setCursor(0, 0);          // Escribe en la posicion del cursor
 
  tft.setTextColor(ST7735_WHITE); //Color de texto
  tft.setTextSize(3);         // Tamaño texto
  tft.print(datos);        // Texto a mostrar
  
  
}

void initializeTFT() {
  // start by disabling both SD and TFT
  pinMode(TFT_CS, OUTPUT);
  digitalWrite(TFT_CS, HIGH);
  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH);

  ss.begin();
  delay(1);

  // Start set the backlight off
  ss.setBacklight(TFTSHIELD_BACKLIGHT_OFF);
  // Reset the TFT
  ss.tftReset();

  // Initialize 1.8" TFT
  tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab

  tft.fillScreen(ST77XX_CYAN); // fill screen cyan

 
  // Set backlight on fully
  // ss.setBacklight(TFTSHIELD_BACKLIGHT_ON);
  // Or you can set the backlight one third on
  // ss.setBacklight(TFTSHIELD_BACKLIGHT_ON / 3);
  // Or dim it up
  for (int32_t i=TFTSHIELD_BACKLIGHT_OFF; i<TFTSHIELD_BACKLIGHT_ON; i+=100) {
    ss.setBacklight(i);
    delay(1);
  }
  delay(1000);
  bmpDraw("/dieect.bmp", 0, 0);
  delay(1000);
  tft.fillScreen(ST77XX_BLACK);
}

void printDirectory(File dir, int numTabs) {
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}

// This function opens a Windows Bitmap (BMP) file and
// displays it at the given coordinates.  It's sped up
// by reading many pixels worth of data at a time
// (rather than pixel by pixel).  Increasing the buffer
// size takes more of the Arduino's precious RAM but
// makes loading a little faster.  20 pixels seems a
// good balance.

#define BUFFPIXEL 20

void bmpDraw(char *filename, uint8_t x, uint16_t y) {

  File     bmpFile;
  int      bmpWidth, bmpHeight;   // W+H in pixels
  uint8_t  bmpDepth;              // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;        // Start of image data in file
  uint32_t rowSize;               // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3*BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
  uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  boolean  goodBmp = false;       // Set to true on valid header parse
  boolean  flip    = true;        // BMP is stored bottom-to-top
  int      w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0, startTime = millis();

  if((x >= tft.width()) || (y >= tft.height())) return;

  Serial.println();
  Serial.print(F("Loading image '"));
  Serial.print(filename);
  Serial.println('\'');

  // Open requested file on SD card
  if ((bmpFile = SD.open(filename)) == NULL) {
    Serial.print(F("File not found"));
    return;
  }

  // Parse BMP header
  if(read16(bmpFile) == 0x4D42) { // BMP signature
    Serial.print(F("File size: ")); Serial.println(read32(bmpFile));
    (void)read32(bmpFile); // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile); // Start of image data
    Serial.print(F("Image Offset: ")); Serial.println(bmpImageoffset, DEC);
    // Read DIB header
    Serial.print(F("Header size: ")); Serial.println(read32(bmpFile));
    bmpWidth  = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    if(read16(bmpFile) == 1) { // # planes -- must be '1'
      bmpDepth = read16(bmpFile); // bits per pixel
      Serial.print(F("Bit Depth: ")); Serial.println(bmpDepth);
      if((bmpDepth == 24) && (read32(bmpFile) == 0)) { // 0 = uncompressed

        goodBmp = true; // Supported BMP format -- proceed!
        Serial.print(F("Image size: "));
        Serial.print(bmpWidth);
        Serial.print('x');
        Serial.println(bmpHeight);

        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * 3 + 3) & ~3;

        // If bmpHeight is negative, image is in top-down order.
        // This is not canon but has been observed in the wild.
        if(bmpHeight < 0) {
          bmpHeight = -bmpHeight;
          flip      = false;
        }

        // Crop area to be loaded
        w = bmpWidth;
        h = bmpHeight;
        if((x+w-1) >= tft.width())  w = tft.width()  - x;
        if((y+h-1) >= tft.height()) h = tft.height() - y;

        // Set TFT address window to clipped image bounds
        tft.startWrite();
        tft.setAddrWindow(x, y, w, h);

        for (row=0; row<h; row++) { // For each scanline...

          // Seek to start of scan line.  It might seem labor-
          // intensive to be doing this on every line, but this
          // method covers a lot of gritty details like cropping
          // and scanline padding.  Also, the seek only takes
          // place if the file position actually needs to change
          // (avoids a lot of cluster math in SD library).
          if(flip) // Bitmap is stored bottom-to-top order (normal BMP)
            pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
          else     // Bitmap is stored top-to-bottom
            pos = bmpImageoffset + row * rowSize;
          if(bmpFile.position() != pos) { // Need seek?
            tft.endWrite();
            bmpFile.seek(pos);
            buffidx = sizeof(sdbuffer); // Force buffer reload
          }

          for (col=0; col<w; col++) { // For each pixel...
            // Time to read more pixel data?
            if (buffidx >= sizeof(sdbuffer)) { // Indeed
              bmpFile.read(sdbuffer, sizeof(sdbuffer));
              buffidx = 0; // Set index to beginning
              tft.startWrite();
            }

            // Convert pixel from BMP to TFT format, push to display
            b = sdbuffer[buffidx++];
            g = sdbuffer[buffidx++];
            r = sdbuffer[buffidx++];
            tft.pushColor(tft.color565(r,g,b));
          } // end pixel
        } // end scanline
        tft.endWrite();
        Serial.print(F("Loaded in "));
        Serial.print(millis() - startTime);
        Serial.println(" ms");
      } // end goodBmp
    }
  }

  bmpFile.close();
  if(!goodBmp) Serial.println(F("BMP format not recognized."));
}

// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t read16(File f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSBSD
  return result;
}

uint32_t read32(File f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}
