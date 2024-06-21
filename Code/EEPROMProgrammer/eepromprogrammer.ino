#include <Arduino.h>

#define WE 13
#define IO0 12
#define IO7 5
#define SHIFT_DATA 4
#define SHIFT_CLK 3
#define SHIFT_REG_CLK 2

unsigned int addressSpace = 8191;
int currentPage;
bool writeMode;
bool validateMode;
bool valid;
int overwritten;
int fails;

char waitingMsg[] = "Waiting for command...";

void printNextPageMsg() {
    Serial.print("Ready for page ");
    Serial.println(currentPage + 1);
}

void setup() {
    // put your setup code here, to run once:
    pinMode(SHIFT_DATA, OUTPUT);
    pinMode(SHIFT_CLK, OUTPUT);

    digitalWrite(SHIFT_REG_CLK, LOW);
    pinMode(SHIFT_REG_CLK, OUTPUT);

    digitalWrite(WE, HIGH);
    pinMode(WE, OUTPUT);

    Serial.begin(115200);
    Serial.println(waitingMsg);
}


// TODO use SPI for faster shiftout -> allow for EEPROM page-write (no more than 150 us between 2 writes)
/*void shiftOutFast(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val) {
    for (unsigned int i = 0; i < 8; i++) {
        if (bitOrder == LSBFIRST) digitalWrite(dataPin, (val & (1 << i)) != 0);
        else digitalWrite(dataPin, (val & (1 << (7 - i))) != 0);
        digitalWrite(clockPin, HIGH);
        digitalWrite(clockPin, LOW);
    }
}*/

/*
 * Output the address bits and outputEnable signal using shift registers.
 */
void setAddress(unsigned int address, bool outputEnable) {
    shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, (address >> 8) | (outputEnable ? 0x00 : 0x80));
    shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, address);

    digitalWrite(SHIFT_REG_CLK, HIGH);
    digitalWrite(SHIFT_REG_CLK, LOW);
}

bool alreadyReading;


/*
 * Read a byte from the EEPROM at the specified address.
 */
byte readByte(unsigned int address) {
    setAddress(address, /*outputEnable*/ true);

    if (!alreadyReading) {
        for (int pin = IO7; pin <= IO0; pin++) {
            pinMode(pin, INPUT);
        }
        alreadyReading = true;
    }

    byte data = 0;
    for (int pin = IO7; pin <= IO0; pin++) {
        data = (data << 1) + digitalRead(pin);
    }
    return data;
}

/*
 * Write a byte to the EEPROM at the specified address.
 *
 * Returns whether the byte had to be overwritten (was different from the specified data)
 */
int writeByte(unsigned int address, byte data) {
    unsigned long startTime = micros();

    // Check if the same data is already present
    if (readByte(address) == data) return false;

    setAddress(address, /*outputEnable*/ false);
    for (int pin = IO0; pin >= IO7; pin--) {
        pinMode(pin, OUTPUT);
    }
    alreadyReading = false;

    byte dataCopy = data;
    for (int pin = IO0; pin >= IO7; pin--) {
        digitalWrite(pin, dataCopy & 1);
        dataCopy = dataCopy >> 1;
    }

    digitalWrite(WE, LOW);
    delayMicroseconds(1);
    digitalWrite(WE, HIGH);

    byte read = readByte(address);
    while (read != data) {
        read = readByte(address);
        if (micros() - startTime > 10000) break;
    }

    // Make sure the write was successful
    if (readByte(address) != data) {
        fails++;
        //Serial.println("Failed to write byte, retrying!");
        writeByte(address, data);
    }
    return true;
}


const unsigned int numChars = 1025;
char receivedChars[numChars]; // an array to store the received data
bool newData;

void receiveSerial() {
    static int index = 0;
    char endMarker = '\n';
    char rc;

    while (Serial.available() > 0 && !newData) {
        rc = Serial.read();

        if (rc != endMarker) {
            receivedChars[index] = rc;
            index++;
            if (index >= numChars) {
                index = numChars - 1;
            }
        } else {
            if (!writeMode && !validateMode) receivedChars[index] = '\0'; // terminate the string
            index = 0;
            newData = true;
        }
    }
}

int inputLength() {
    int length = 0;
    for (char receivedChar : receivedChars) {
        if (receivedChar == '\0') break;
        length++;
    }
    return length;
}


/*
 * Read the contents of the EEPROM and print them to serial
 */
void printContents() {
    for (int addr = 0; addr <= addressSpace; addr += 16) {
        byte data[16];
        for (int offset = 0; offset <= 15; offset += 1) {
            data[offset] = readByte(addr + offset);
        }

        char buf[60];
        sprintf(buf, "%04x:  %02x %02x %02x %02x %02x %02x %02x %02x   %02x %02x %02x %02x %02x %02x %02x %02x",
                addr, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
                data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);

        Serial.println(buf);
    }
    Serial.println(waitingMsg);
}

/*
 * Write/validate the given serial input to the EEPROM, 512 bytes at a time
 */
void writeContents(bool write, bool validate) {
    int charIndex = 0;
    unsigned long startTime = millis();
    for (unsigned int addr = currentPage * 512; addr <= 511 + currentPage * 512; addr++) {
        if (addr >= addressSpace) {
            writeMode = false;
            validateMode = false;
            if (addr > addressSpace) break;
        }
        char hex[3] = {'f', 'f', '\0'};
        hex[0] = receivedChars[charIndex];
        hex[1] = receivedChars[charIndex + 1];
        byte data = (byte) strtol(hex, nullptr, 16);
        charIndex += 2;
        if (write) {
            if (writeByte(addr, data)) overwritten++;
            /*if (charIndex % 256 == 0) {
                Serial.print((charIndex / 256) * 25);
                Serial.println("%");
            }*/
        }
        if (validate) {
            byte read = readByte(addr);
            if (read != data) {
                char buf[55];
                sprintf(buf, "Data invalid at 0x%04x\nStored: 0x%02x\nExpected: 0x%02x", addr, read, data);
                Serial.println(buf);
                valid = false;
                validateMode = false;
                if (1023 + currentPage * 1024 >= addressSpace) validateMode = false;
                break;
            }
        }
    }
    currentPage++;

    if (write) {
        Serial.print("Writing page took ");
        Serial.print(millis() - startTime);
        Serial.println(" ms");
    }

    if (write && !writeMode) {
        Serial.print("Failed and retried ");
        Serial.print(fails);
        Serial.println(" times");
        Serial.print("Overwritten ");
        Serial.print(overwritten);
        Serial.println(" bytes");
        Serial.print("Write complete! ");
        Serial.println(waitingMsg);
    }

    if (validate && !validateMode) {
        if (valid) Serial.println("Data valid");
        Serial.print("Validation complete! ");
        Serial.println(waitingMsg);
    }

    if (writeMode || validateMode) {
        printNextPageMsg();
    }
}


void showSizeHelp(bool nonDigit, bool tooLong) {
    if (nonDigit) Serial.println("Digits only (0-9;a-f)!");
    if (tooLong) Serial.println("5 digits max!");
    //if (tooBig) Serial.println("Error: Size must not be bigger than 32767!");
    Serial.println("Usage: size <address-space-size-in-bytes>");
    //Serial.println("Some example sizes:");
    //Serial.println("AT28C16: 2047 (0x7FF)");
    //Serial.println("AT28C64: 8191 (0x3FFF)");
    //Serial.println("AT28C256: 32767 (0x7FFF) (default)");
    newData = false;
    memset(receivedChars, 'f', 1025);
}

void processInput() {
    if (newData != 0) {
        int length = inputLength();

        if (writeMode) {
            writeContents(true, false);
        } else if (validateMode) {
            writeContents(false, true);
        } else if (strncmp(receivedChars, "size", 4) == 0) {
            if (length > 10) {
                showSizeHelp(false, true);
                return;
            } else if (length > 5) {
                char size[8];
                for (int i = 5; i <= length; ++i) {
                    if (i == length) {
                        size[i] = '\0';
                        break;
                    }
                    char recvChar = receivedChars[i];
                    if ((recvChar >= '0' && recvChar <= '9') || (recvChar >= 'a' && recvChar <= 'f') ||
                        (recvChar >= 'A' && recvChar <= 'F') || recvChar == 'x')
                        size[i - 5] = recvChar;
                    else {
                        showSizeHelp(true, false);
                        return;
                    }
                }
                addressSpace = strtol(size, nullptr, 0) - 1;
                currentPage = 0;
                //Serial.println(addressSpace + 1);
                Serial.print("Successfully set address space size to ");
                Serial.print(addressSpace + 1);
                Serial.println(" bytes!");
            } else showSizeHelp(false, false);
        } else if (strcmp(receivedChars, "print") == 0) {
            printContents();
        } else if (strcmp(receivedChars, "write") == 0) {
            currentPage = 0;
            overwritten = 0;
            fails = 0;
            writeMode = true;
            printNextPageMsg();
        } else if (strcmp(receivedChars, "validate") == 0) {
            currentPage = 0;
            validateMode = true;
            valid = true;
            printNextPageMsg();
        } else {
            Serial.print("Unknown command: ");
            Serial.println(receivedChars);
        }
        memset(receivedChars, 'f', 1025);
        newData = false;
    }
}

void loop() {
    // put your main code here, to run repeatedly:
    receiveSerial();
    processInput();
}
