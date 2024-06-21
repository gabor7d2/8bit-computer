
#define WE 13
#define IO0 12
#define IO7 5
#define SHIFT_DATA 4
#define SHIFT_CLK 3
#define SHIFT_REG_CLK 2

#define EEPROM_AT28C256 32767
#define EEPROM_AT28C64 8191
#define EEPROM_AT28C16 2047

unsigned int addressSpace;

/*
 * Output the address bits and outputEnable signal using shift registers.
 */
void setAddress(int address, bool outputEnable) {
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, (address >> 8) | (outputEnable ? 0x00 : 0x80));
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, address);

  digitalWrite(SHIFT_REG_CLK, LOW);
  digitalWrite(SHIFT_REG_CLK, HIGH);
  digitalWrite(SHIFT_REG_CLK, LOW);
}


/*
 * Read a byte from the EEPROM at the specified address.
 */
byte readByte(int address) {
  for (int pin = IO7; pin <= IO0; pin++) {
    pinMode(pin, INPUT);
  }
  setAddress(address, /*outputEnable*/ true);

  byte data = 0;
  for (int pin = IO7; pin <= IO0; pin++) {
    data = (data << 1) + digitalRead(pin);
  }
  return data;
}


/*
 * Write a byte to the EEPROM at the specified address.
 */
void writeByte(int address, byte data) {
  setAddress(address, /*outputEnable*/ false);
  for (int pin = IO0; pin >= IO7; pin--) {
    pinMode(pin, OUTPUT);
  }

  for (int pin = IO0; pin >= IO7; pin--) {
    digitalWrite(pin, data & 1);
    data = data >> 1;
  }
  digitalWrite(WE, LOW);
  delayMicroseconds(1);
  digitalWrite(WE, HIGH);
  delay(10);
}


/*
 * Read the contents of the EEPROM and print them to the serial monitor.
 */
void printContents() {
  for (int base = 0; base <= addressSpace; base += 16) {
    byte data[16];
    for (int offset = 0; offset <= 15; offset += 1) {
      data[offset] = readByte(base + offset);
    }

    char buf[80];
    sprintf(buf, "%04x:  %02x %02x %02x %02x %02x %02x %02x %02x   %02x %02x %02x %02x %02x %02x %02x %02x",
            base, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
            data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);

    Serial.println(buf);
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(WE, OUTPUT);
  pinMode(SHIFT_DATA, OUTPUT);
  pinMode(SHIFT_CLK, OUTPUT);
  pinMode(SHIFT_REG_CLK, OUTPUT);
  digitalWrite(WE, HIGH);

  addressSpace = EEPROM_AT28C256;
  
  Serial.begin(256000);
  Serial.println("Waiting for input...");

  /*for (int base = 0; base <= 255; base += 16) {
    for (int offset = 0; offset <= 15; offset += 1) {
      writeByte(base + offset, base + offset);
    }
  }*/

  //Serial.println("Reading contents of EEPROM...");
  //printContents();
}

const byte numChars = 255;
char receivedChars[numChars];   // an array to store the received data

boolean newData = false;

void loop() {
  // put your main code here, to run repeatedly:
  recvWithEndMarker();
  showNewData();
}

void recvWithEndMarker() {
    static int ndx = 0;
    char endMarker = '\n';
    char rc;
   
    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (rc != endMarker) {
            receivedChars[ndx] = rc;
            ndx++;
            if (ndx >= numChars) {
                ndx = numChars - 1;
            }
        }
        else {
            receivedChars[ndx] = '\0'; // terminate the string
            ndx = 0;
            newData = true;
        }
    }
}

void showNewData() {
    if (newData == true) {
        Serial.print("This just in ... ");
        Serial.println(receivedChars);
        newData = false;
    }
}
