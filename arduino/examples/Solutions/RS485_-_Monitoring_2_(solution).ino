#define RED_PIN A0  
#define GREEN_PIN A1
#define BLUE_PIN A2

//HardwareSerial pins
#define TX_PIN 7                  
#define RX_PIN 8  

void setup() {
    Serial1.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);  // initialize serial port
    delay(1000);          // wait for serial port to initialize

    pinMode(GREEN_PIN, OUTPUT);    	// Led verde encendido significa que el puerto serie esta libre
    pinMode(BLUE_PIN, OUTPUT);     	// Led azul no se usará de momento
    pinMode(RED_PIN, OUTPUT);      	// Led rojo encendido significa que el puerto serie esta ocupado
    analogWrite(GREEN_PIN, 255);    	// encender led verde
    analogWrite(BLUE_PIN, 0);   	// apagar led azul  
    analogWrite(RED_PIN, 0);		// apagar led rojo
}

int temperature;     				// variable para almacenar la temperatura leida

void loop() {
	
	temperature = 0;     				// inicializar temperatura
	
	if(Serial1.available()){              	// hay datos para leer en el puerto serie
		analogWrite(GREEN_PIN, 255);  // apagar led verde
		analogWrite(RED_PIN,0);       // encender led rojo
		delay(200);
		temperature=int(Serial1.read());
		Serial1.write(100);
		Serial1.flush();
		delay(100);                           
		analogWrite(GREEN_PIN, 0);    		// encender el led verde
		analogWrite(RED_PIN, 255);   			// apagar el led rojo 
	}
}
