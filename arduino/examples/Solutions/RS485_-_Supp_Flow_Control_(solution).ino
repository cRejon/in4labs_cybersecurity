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
		if (Serial1.read()==START_CHAR){		// detect the start of the frame
			if (Serial1.read()==TEMP_CMD){		// check if there is a temperature request
				if (Serial1.read()==END_CHAR){	// receive the end, so the frame is correct
                    delay(2000);                // wait two seconds before sending the data back
					Serial1.write(START_CHAR);
					Serial1.write(100);         // this is the fake data
					Serial1.write(END_CHAR);
					Serial1.flush();
				}
			}
		}
	}

    delay(200);
}
