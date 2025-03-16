#include <LiquidCrystal.h>      // include LCD library

// ===== Configuration Constants =====
// Device addressing
#define HOST_DIR 100
#define BROADCAST_ADDR 255
#define MAX_SLAVES 5

// Protocol identifiers
#define START_CHAR 'I'
#define END_CHAR 'F'

// Command types
#define HELLO_CMD 'H'
#define UPDATE_CMD 'U'
#define TEMP_CMD 'T'

// Timing parameters (ms)
#define SERIAL_TIMEOUT 1000
#define MESSAGE_DELAY 100
#define MASTER_REQUEST_DELAY 500
#define TEMP_REQUEST_DELAY 10000

// Hardware configuration
#define TX_PIN 7
#define RX_PIN 8 

// ===== Global Variables =====
unsigned long previousMillis = 0;
uint8_t slaveAddresses[MAX_SLAVES] = {0};
uint8_t currentSlaveIndex = 0;
uint8_t registeredSlaves = 0; 

const int rs = 3, en = 4, d4 = 5, d5 = 6, d6 = 9, d7 = 10;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// ===== Message Structure =====
struct Message {
    char start;
    uint8_t origin;  
    uint8_t destination;
    char command;
    char end;
};

// ===== Initialization =====
void setup() {
    Serial1.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);  // initialize serial port
    Serial1.setTimeout(SERIAL_TIMEOUT);
    while (!Serial1) {
        ; // wait for serial port connection
    }

    lcd.begin(16, 2);           // set up the LCD's number of columns and rows
    lcd.clear();

    // Optional: Show initialization message
    lcd.print("RS485 MitM Ready");
}

// ===== Communication Functions =====
bool readMessage(Message& msg) {
    // Wait for start character
    if (!Serial1.find(START_CHAR)) return false;
    
    // Parse message components
    msg.start = START_CHAR;
    msg.origin = Serial1.parseInt();
    msg.destination = Serial1.parseInt();
    msg.command = Serial1.read();
    msg.end = Serial1.read();
    
    return msg.start == START_CHAR && msg.end == END_CHAR;
}

void sendMessage(uint8_t origin, uint8_t destination, char command) {
    clearSerialBuffer();  // Clear before sending
    
    Serial1.write(START_CHAR);
    Serial1.print(origin);
    Serial1.print(destination);
    Serial1.write(command);
    Serial1.write(END_CHAR);
    Serial1.flush();  // Ensure message is sent completely
}

void clearSerialBuffer() {
    while (Serial1.available()) {
        Serial1.read();
    }
}

// ===== Slave Management =====
bool registerSlave(uint8_t address) {
    for (uint8_t i = 0; i < MAX_SLAVES; i++) {
        if (slaveAddresses[i] == address) return true;
        if (slaveAddresses[i] == 0) {
            slaveAddresses[i] = address;
            registeredSlaves++;  
            return true;
        }
    }
    return false;
}

// ===== Display Functions =====
void updateLCD(uint8_t address, int temperature) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Slave: ");
    lcd.print(address);
    lcd.setCursor(0, 1);
    lcd.print("Temp: ");
    lcd.print(temperature);
    lcd.print(" C");
}

// ===== Main Program Loop =====
void loop() {
    Message msg;   
    unsigned long currentMillis = millis();              

    // Handle incoming messages
    if (Serial1.available()) {
        delay(MESSAGE_DELAY);  // Give time for the full message to arrive
        if (readMessage(msg)) {
            if (msg.destination == BROADCAST_ADDR && msg.command == HELLO_CMD) {
                if (registerSlave(msg.origin)) {
                    sendMessage(HOST_DIR, msg.origin, HELLO_CMD);
                }
            }
        }
    }

    // Temperature request cycle
    if (currentMillis - previousMillis >= TEMP_REQUEST_DELAY && registeredSlaves > 0) {    
        previousMillis = currentMillis;

        sendMessage(HOST_DIR, slaveAddresses[currentSlaveIndex], TEMP_CMD);
        delay(MASTER_REQUEST_DELAY);

        if (Serial1.available()) {
            delay(MESSAGE_DELAY);
            if (readMessage(msg)) {
                if (msg.origin == slaveAddresses[currentSlaveIndex] && msg.destination == HOST_DIR) {
                    updateLCD(msg.origin, msg.command);
                }
            }
        }

        currentSlaveIndex = (currentSlaveIndex + 1) % registeredSlaves;  
    }
}
