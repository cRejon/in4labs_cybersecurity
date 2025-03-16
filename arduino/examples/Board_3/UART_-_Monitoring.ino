#define AR1_PIN 1
#define AR2_PIN 0

void setup() {
  	// Initialize Serial for output
  	Serial.begin(9600);

	Serial1.begin(9600, SERIAL_8N1, AR1_PIN, 5);
	while (!Serial1) {
		; 
	}
	Serial2.begin(9600, SERIAL_8N1, AR2_PIN, 6);
	while (!Serial2) {
		; 
	}

}


void loop() {
    // your code here


  
  delay(100); // Small delay to avoid overwhelming the Serial output
}
