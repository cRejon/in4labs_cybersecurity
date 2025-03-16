// ===== Configuration Constants =====
// Supplanting value 
#define SUPP_VAL 100

// Device addressing
#define HOST_DIR 200
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
#define UPDATE_REQUEST_DELAY 500

// Hardware configuration
#define TX_PIN 7
#define RX_PIN 8 

// ===== Global Variables =====
uint8_t master_address = 0;
uint8_t slave_address = 0;
bool has_addresses = false;

// ===== Message Structure =====
struct Message {
    char start;
    uint8_t origin;  
    uint8_t destination;
    char command;
    char end;
};

// ===== Exercise Functions =====
bool discoverAddresses(Message msg, uint8_t& master_address, uint8_t& slave_address) {
    if (msg.command == TEMP_CMD) {
        master_address = msg.origin;
        slave_address = msg.destination;
        return true;
    }
    return false;
}

void updateMaster() {
    sendMessage(HOST_DIR, BROADCAST_ADDR, UPDATE_CMD);
}

bool supplantMessage(Message msg, uint8_t& master_address, uint8_t& slave_address) {
    if (msg.command == TEMP_CMD) {
        sendMessage(slave_address, master_address, SUPP_VAL);
        return true;
    }
    return false;
}

// ===== Initialization =====
void setup() {
    Serial1.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);  // initialize serial port
    Serial1.setTimeout(SERIAL_TIMEOUT);
    while (!Serial1) {
        ; // wait for serial port connection
    }
}

// ===== Main Program Loop =====
void loop() {
    Message msg;   

    // Handle incoming messages
    if (Serial1.available()) {
        delay(MESSAGE_DELAY);  // Give time for the full message to arrive
        if (readMessage(msg)) {
            if (!has_addresses) {
                if (discoverAddresses(msg, master_address, slave_address)) {
                    delay(UPDATE_REQUEST_DELAY);
                    updateMaster();
                    has_addresses = true;
                }
            } else {
                supplantMessage(msg, master_address, slave_address);
            }
        }
    }

    delay(500);  // Delay to prevent spamming the serial port
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
