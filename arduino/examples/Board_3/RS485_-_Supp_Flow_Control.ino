// Protocol constants
#define START_CHAR 'I'
#define TEMP_CMD 'T'
#define END_CHAR 'F'

// RS485 pins
#define TX_PIN 7                  
#define RX_PIN 8  


void setup() {
    Serial1.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);  // initialize serial port
    while (!Serial1) {
        ; // wait for serial port connection
    }
}

void loop() {
    if (Serial1.available()){
		delay(100);
		// your code here
        
	}

    delay(200);
}
