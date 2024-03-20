#define RED_PIN 5  
#define GREEN_PIN 6
#define BLUE_PIN 9

void setup() {
	Serial1.begin(115200);  //configuración de la frecuencia del puerto serie
	while (!Serial1) {
		; 
	}

	pinMode(RED_PIN, OUTPUT);    // Led verde encendido significa que el puerto serie esta libre
	pinMode(GREEN_PIN, OUTPUT);     // Led azul no se usará de momento
	pinMode(BLUE_PIN, OUTPUT);      // Led rojo encendido significa que el puerto serie esta ocupado
	analogWrite(RED_PIN, 255);    // encender led verde
	analogWrite(GREEN_PIN, 0);   // apagar led azul  
	analogWrite(BLUE_PIN, 0);	// apagar led rojo
}


void loop() {
	
}
