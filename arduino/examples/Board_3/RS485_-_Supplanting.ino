// RS-485 pins
#define TX_PIN 7       
#define RX_PIN 8  


void setup() {
  	Serial.begin(9600); // initialize serial port for Serial Monitor

    Serial1.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);  // initialize serial 1 port for RS-485
    while (!Serial1) {
        ; // wait for serial port connection
    }
}

int temperature = 0;     				// variable to store the read temperature

void loop() {

	if(Serial1.available()){              	// there is data to read in the serial port
		// your code here

	}

    delay(500); // wait for a while
}
