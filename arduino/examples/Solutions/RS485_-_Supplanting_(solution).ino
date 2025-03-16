// RS-485 pins
#define TX_PIN 7       
#define RX_PIN 8  


void setup() {
  	Serial.begin(9600); 

    Serial1.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);  
    while (!Serial1) {
        ; 
    }
}

int temperature = 0;     				

void loop() {
    
	if(Serial1.available()){              	
		temperature=int(Serial1.read());

        Serial.print("Data : ");
        Serial.println(temperature);
        
        delay(2000);  // wait two seconds before sending the data back

        Serial1.write(100); // this is the fake data
		Serial1.flush();
	}

    delay(500);
}
