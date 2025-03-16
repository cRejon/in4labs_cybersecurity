
void setup() {
    // Initialize Serial
	Serial0.begin(9600);  
	while (!Serial0) {
		; 
	}
}

void loop() {
    delay(10);										
	if (Serial0.available()){
		if (Serial0.read()=='T') {
            Serial0.write(99);						
        }
	}
}
