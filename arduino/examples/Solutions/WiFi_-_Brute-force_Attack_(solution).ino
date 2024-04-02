#include <WiFiNINA.h> 				//Declaro la libreria WiFi
#include <LiquidCrystal.h>      // include LCD library

#define RED_PIN 5  
#define GREEN_PIN 6
#define BLUE_PIN 9

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 17, en = 16, d4 = 15, d5 = 14, d6 = 4, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

char ssid[] = "WifiNINA-Access-Point";  // SSID a conectar
#define tam_clave 7
char* pwd[tam_clave]={"admin","adminttd","1234","admin1234","123456789","123456","admin123456789"};	//defino listado de contraseñas

void setup() {  
  WiFi.config(IPAddress(192, 168, 4, 3));	//Defino Ip del cliente
  
  pinMode(GREEN_PIN, OUTPUT);      // Se define el pin del led verde como salida
  pinMode(BLUE_PIN, OUTPUT);       // Se define el pin del led azul como salida
  pinMode(RED_PIN, OUTPUT);        // Se define el pin del led rojo como salida
  digitalWrite(GREEN_PIN, 255);      // Se enciende el led verde
  digitalWrite(BLUE_PIN, 0);     // Se apaga el led azul
  digitalWrite(RED_PIN, 0);		// Se apaga el led rojo

  lcd.begin(16, 2);           // set up the LCD's number of columns and rows
  lcd.clear(); 
}

String datos;
int status = WL_IDLE_STATUS;

void loop() {
  datos="No conectado!!";
  updateLCD();							//muestra mensaje mientras busca
  delay(2000);
  for (int i=0;i<tam_clave;i++) {
	digitalWrite(GREEN_PIN, 0);      // Apaga el led verde
	digitalWrite(BLUE_PIN, 255);     	// Enciende el led azul
	status = WiFi.begin(ssid, pwd[i]);      // Intenta conectar al ssid con la clave de indice i
	delay(5000);                          	// Espera 5 seg a la conexion
	digitalWrite(GREEN_PIN, 255);      	// Se enciende el led verde
	digitalWrite(BLUE_PIN, 0);     	// Se apaga el led azul
	if (status==WL_CONNECTED) {				// Si el estado es conectado
		digitalWrite(GREEN_PIN, 0);  // Apaga el led verde
		digitalWrite(RED_PIN, 255);     	// Enciende el led rojo
		datos="Conectado!!";
		datos+="PWD=";
		datos+=pwd[i];
		updateLCD();						//Saca por pantalla "Conectado!!" y la clave de conexion
		break;
	}
  }
  delay(20000);                     		//espera de 20 segundos  
}

// function that updates the LCD screen
void updateLCD() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(datos);
}
