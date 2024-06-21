
const int DATA_0 = 5;
const int DATA_1 = 6;
const int DATA_2 = 7;
const int DATA_3 = 8;
const int DATA_4 = 9;
const int DATA_5 = 10;
const int DATA_6 = 11;
const int DATA_7 = 12;
const int WE_PIN = 4;
const int CLOCK_PIN = 3;
const int BTN_PIN = 2;

const int timeMultiplier = 10;
int index = 0;
unsigned long prevTime = 0;

void setup() {
  pinMode(DATA_0, OUTPUT);
  pinMode(DATA_1, OUTPUT);
  pinMode(DATA_2, OUTPUT);
  pinMode(DATA_3, OUTPUT);
  pinMode(DATA_4, OUTPUT);
  pinMode(DATA_5, OUTPUT);
  pinMode(DATA_6, OUTPUT);
  pinMode(DATA_7, OUTPUT);
  pinMode(WE_PIN, OUTPUT);

  pinMode(CLOCK_PIN, OUTPUT);
  TCCR2A = 0x23;
  TCCR2B = 0x09;
  OCR2A = 2;
  // 2 - C4
  // 3 - 
  OCR2B = 1;

  digitalWrite(WE_PIN, HIGH);
  SilenceAllChannels();
  prevTime = millis();

  Serial.begin(115200);
}

void loop() {
  // boci boci tarka
  byte instructions[] = {
    0b10000000, 0b00101000, // C4
    0b10000000, 0b00100000, // E4
    0b10000000, 0b00101000, // C4
    0b10000000, 0b00100000, // E4
    0b10000000, 0b00011011, // G4
    0b10000000, 0b00011011, // G4
    
    0b10000000, 0b00101000, // C4
    0b10000000, 0b00100000, // E4
    0b10000000, 0b00101000, // C4
    0b10000000, 0b00100000, // E4
    0b10000000, 0b00011011, // G4
    0b10000000, 0b00011011, // G4

    0b10000000, 0b00010100, // C5
    0b10000000, 0b00010101, // B4
    0b10000000, 0b00011000, // A4
    0b10000000, 0b00011011, // G4
    0b10000000, 0b00011110, // F4
    0b10000000, 0b00011000, // A4

    0b10000000, 0b00011011, // G4
    0b10000000, 0b00011110, // F4
    0b10000000, 0b00100000, // E4
    0b10000000, 0b00100011, // D4
    0b10000000, 0b00101000, // C4
    0b10000000, 0b00101000, // C4
  };
  for (int i = 0; i < 4; i++) {
    SendByte(0b10010000);

    // first
    SendByte(instructions[0 + i * 12]);
    SendByte(instructions[1 + i * 12]);
    delay(300);

    // second
    SendByte(instructions[2 + i * 12]);
    SendByte(instructions[3 + i * 12]);
    delay(300);

    // first
    SendByte(instructions[4 + i * 12]);
    SendByte(instructions[5 + i * 12]);
    delay(300);

    // second
    SendByte(instructions[6 + i * 12]);
    SendByte(instructions[7 + i * 12]);
    delay(300);

    // third
    SendByte(instructions[8 + i * 12]);
    SendByte(instructions[9 + i * 12]);
    delay(300);

    // fade out
    SendByte(0b10010011);
    delay(100);
    SendByte(0b10010111);
    delay(100);
    SendByte(0b10011011);
    delay(100);

    // fade to max
    SendByte(0b10010000);

    // third
    SendByte(instructions[10 + i * 12]);
    SendByte(instructions[11 + i * 12]);
    delay(300);

    // fade out
    SendByte(0b10010011);
    delay(100);
    SendByte(0b10010111);
    delay(100);
    SendByte(0b10011011);
    delay(100);
  }
}

void SilenceAllChannels()
{
  SendByte(0x9f);
  SendByte(0xbf);
  SendByte(0xdf);
  SendByte(0xff);
}

void SendByte(byte b)
{
  digitalWrite(DATA_0, (b & 1) ? HIGH : LOW);
  digitalWrite(DATA_1, (b & 2) ? HIGH : LOW);
  digitalWrite(DATA_2, (b & 4) ? HIGH : LOW);
  digitalWrite(DATA_3, (b & 8) ? HIGH : LOW);
  digitalWrite(DATA_4, (b & 16) ? HIGH : LOW);
  digitalWrite(DATA_5, (b & 32) ? HIGH : LOW);
  digitalWrite(DATA_6, (b & 64) ? HIGH : LOW);
  digitalWrite(DATA_7, (b & 128) ? HIGH : LOW);
  digitalWrite(WE_PIN, LOW);
  delay(1);
  digitalWrite(WE_PIN, HIGH);
  delay(1);
}
