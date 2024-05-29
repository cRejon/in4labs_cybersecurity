#define RED_PIN A0  
#define GREEN_PIN A1
#define BLUE_PIN A2

void setup() {
	Serial0.begin(115200);  //configuración de la frecuencia del puerto serie
	while (!Serial0) {
		; 
	}

	pinMode(RED_PIN, OUTPUT);    // Led verde encendido significa que el puerto serie esta libre
	pinMode(GREEN_PIN, OUTPUT);     // Led azul no se usará de momento
	pinMode(BLUE_PIN, OUTPUT);      // Led rojo encendido significa que el puerto serie esta ocupado
	analogWrite(RED_PIN, 255);    // encender led verde
	analogWrite(GREEN_PIN, 0);   // apagar led azul  
	analogWrite(BLUE_PIN, 0);	// apagar led rojo
}

int temperature;     				// variable para almacenar la temperatura leida

void loop() {
	
	if (Serial0.available()){
		delay(100);										// Esperar a que el valor este disponible
		if (Serial0.read()=='T') {
			analogWrite(GREEN_PIN, 0);   		// apagar led verde
			analogWrite(RED_PIN, 255);       		// encender led rojo
			delay(500);
			analogWrite(GREEN_PIN, 255);    		// encender el led verde
			analogWrite(RED_PIN, 0);   			// apagar el led rojo 
        }
	}
}
