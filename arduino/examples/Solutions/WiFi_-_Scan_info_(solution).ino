#include <WiFiNINA.h> 				//Declaro la libreria WiFi
#include <LiquidCrystal.h>      // include LCD library

#define RED_PIN 5  
#define GREEN_PIN 6
#define BLUE_PIN 9

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 17, en = 16, d4 = 15, d5 = 14, d6 = 4, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {  
  Wire.begin();              

  pinMode(GREEN_PIN, OUTPUT);      // Se define el pin del led verde como salida
  pinMode(BLUE_PIN, OUTPUT);       // Se define el pin del led azul como salida
  pinMode(RED_PIN, OUTPUT);        // Se define el pin del led rojo como salida
  digitalWrite(GREEN_PIN, 0255;      // Se enciende el led verde
  digitalWrite(BLUE_PIN, 0);     // Se apaga el led azul
  digitalWrite(RED_PIN, 0);		// Se apaga el led rojo

  lcd.begin(16, 2);           // set up the LCD's number of columns and rows
  lcd.clear(); 
}

String datos;
String SSID;               

void loop() {
  datos="Buscando redes";
  updateLCD();						//muestra mensaje mientras busca
  delay(2000);
  int n_SSID=WiFi.scanNetworks();	//escanea en busca de redes 
  if (n_SSID!=-1) {					//si ha encontrado algo
	datos="Num Redes=";
	datos+=n_SSID;
	for (int i=0;i<n_SSID;i++){		//bucle que recorre el total de redes
		datos="Red=";
		datos+=i;
		datos+="\n";
		datos+=WiFi.SSID(i);		//devuelve el nombre de la red
		datos+="\n";
		datos+="Encrip T=";
        tipo_enc(WiFi.encryptionType(i));
		updateLCD();
		delay(3000);
	}
  }
  delay(20000);                     //espera de 20 segundos  
}

void tipo_enc(int thisType) {
  //método para pasar el valor contante que indica el tipo de cifrado a texto
  switch (thisType) {
  	case ENC_TYPE_WEP:
      		datos+= "WEP";
      		break;
    	case ENC_TYPE_TKIP:
      		datos+= "WPA";
      	break;
    	case ENC_TYPE_CCMP:
      		datos+= "WPA2";
      		break;
    	case ENC_TYPE_NONE:
      		datos+="None";
      		break;
    	case ENC_TYPE_AUTO:
      		datos+= "Auto";
      		break;
    	case ENC_TYPE_UNKNOWN:
    	default:
      		datos+= "Unknown";
      		break;
  }
}


// function that updates the LCD screen
void updateLCD() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(datos);
}
