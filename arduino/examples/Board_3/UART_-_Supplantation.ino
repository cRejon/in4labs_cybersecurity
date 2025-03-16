
void setup() {
	Serial0.begin(9600);  
	while (!Serial0) {
		; 
	}
}

void loop() {
    delay(100);
	if (Serial0.available()){
		// your code here
        
	}
}
