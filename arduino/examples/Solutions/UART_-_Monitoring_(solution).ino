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
// Check if data is available on Serial1
  if (Serial1.available()) {
    // Read data from Serial1
    String data1 = Serial1.readString();
    // Output data to Serial
    Serial.print("AR1 data: ");
    Serial.println(data1);
  }

  // Check if data is available on Serial2
  if (Serial2.available()) {
    // Read data from Serial2
    String data2 = Serial2.readString();
    // Output data to Serial
    Serial.print("AR2 data: ");
    Serial.println(data2);
  }

  // Small delay to avoid overwhelming the Serial output
  delay(100);
}
