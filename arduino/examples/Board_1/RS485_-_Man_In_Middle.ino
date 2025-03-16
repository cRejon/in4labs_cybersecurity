#include <DHT.h>

// ===== Configuration Constants =====
// Device addressing
#define HOST_DIR 105
#define BROADCAST_ADDR 255

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

// Hardware configuration
#define TX_PIN 7
#define RX_PIN 8
#define DATA_PIN 2
#define DHT_TYPE DHT22

// ===== Global Variables =====
DHT dht(DATA_PIN, DHT_TYPE);
int master_dir = 0;  // Master device address (0 = not yet discovered)

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
    Serial1.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
    Serial1.setTimeout(SERIAL_TIMEOUT);
    
    dht.begin()
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

// ===== Master Discovery =====
void discoverMaster() {
    sendMessage(HOST_DIR, BROADCAST_ADDR, HELLO_CMD);
    delay(MASTER_REQUEST_DELAY);
    
    Message msg;
    if (Serial1.available() && readMessage(msg)) {
        if (msg.destination == HOST_DIR && msg.command == HELLO_CMD) {
            master_dir = msg.origin;
        }
    }
    clearSerialBuffer();
}

// ===== Main Program Loop =====
void loop() {
    // If master not discovered yet, try to find it
    if (master_dir == 0) {
        discoverMaster();
        return;
    }
    
    // Process incoming messages
    if (Serial1.available()) {
        delay(MESSAGE_DELAY);  // Give time for the full message to arrive
        
        Message msg;
        if (readMessage(msg)) {
            // Process messages from master directed to us
            if (msg.origin == master_dir && msg.destination == HOST_DIR) {
                if (msg.command == TEMP_CMD) {
                    float temp = dht.readTemperature();
                    if (!isnan(temp)) {
                        sendMessage(HOST_DIR, master_dir, int(temp));
                    };
                }
            } 
            // Process broadcast messages
            else if (msg.destination == BROADCAST_ADDR && msg.command == UPDATE_CMD) {
                master_dir = msg.origin;  // Update master address
            }
        }
        clearSerialBuffer();  // Clear any remaining data
    }
}