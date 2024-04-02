#include <ArduinoBLE.h>
#include <LiquidCrystal.h>      // include LCD library

#define RED_PIN 5  
#define GREEN_PIN 6
#define BLUE_PIN 9

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 17, en = 16, d4 = 15, d5 = 14, d6 = 4, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

String datos;
BLEDevice peripheral; 

void setup() {
  pinMode(GREEN_PIN, OUTPUT);    	// Led verde encendido significa que la placa de enciende
  pinMode(BLUE_PIN, OUTPUT);     	// Led azul encendido significará que esta buscando UUID
  pinMode(RED_PIN, OUTPUT);      	// Led rojo encendido significa que ha encontrado un UUID
  digitalWrite(GREEN_PIN, 255);    	// encender led verde
  digitalWrite(BLUE_PIN, 0);   	// apagar led azul  
  digitalWrite(RED_PIN, 0);		// apagar led rojo

  
  if (!BLE.begin()) {                   // Inicialización de BLE
    while (1);
  }
}

char dir_uuid[4];			//almacencamiento de la dirección BLE a buscar

void loop() {
  for(int i=0;i<=9999;i++) {
	sprintf(dir_uuid, "%04d", i);		// Conviero el entero a una cadena de 4 cifras numéricas
    BLE.scanForUuid(dir_uuid);      	// Escanea dir
	peripheral = BLE.available(); 		// Devuelve verdadero si descubre algun dispositivo
	int counter = 0;              		// Establecer a 0 el numero de intentos de conexion
   	digitalWrite(GREEN_PIN, 0); 	// Apaga el LED VERDE
	digitalWrite(BLUE_PIN, 255);   	// Enciende el led azul para indicar que esta buscando UUID  
	digitalWrite(RED_PIN, 0);	// Apaga el LED ROJO
	while(!peripheral && counter < 10){	// mientras no se decubra y no se llegue a 10 intentos
		BLE.scanForUuid(dir_uuid);      // busco el dispositivo
		delay(10);        
		peripheral = BLE.available();   // Devuelve verdadero si descubre algun dispositivo        
		counter =counter+1;             // Incrementar el contador
	}  
	BLE.stopScan();						// Parar el escaneo
	digitalWrite(GREEN_PIN, 255);    // Enciende el LED VERDE
  	digitalWrite(BLUE_PIN, 0);   // Apaga el LED AZUL- ha parado la busqueda  
  	digitalWrite(RED_PIN, 0);	// Apaga el LED ROJO
	if (peripheral) {					// Si ha encontrado un periférico
		datos=dir_uuid;
		if (peripheral.hasLocalName()) {		// Si tiene nombre local
			digitalWrite(GREEN_PIN, 0);  // Apaga el LED VERDE
  			digitalWrite(BLUE_PIN, 0);   // Apaga el LED AZUL  
  			digitalWrite(RED_PIN, 255);		// Enciende LED ROJO-Dispositivo encontrado 
			datos+="-";
			datos+=peripheral.localName();
			updateLCD();						//actualizo la pantalla
			delay(20000);
			digitalWrite(GREEN_PIN, 255);    // Enciende LED VERDE
  			digitalWrite(BLUE_PIN, 0);   // Apaga el LED AZUL  
  			digitalWrite(RED_PIN, 0);	// Apaga LED ROJO
			break;
		}
	} 
  }
}

// function that updates the LCD screen
void updateLCD() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(datos);
}