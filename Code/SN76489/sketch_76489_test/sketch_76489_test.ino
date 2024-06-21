
const int DATA_0 = 10;
const int DATA_1 = 8;
const int DATA_2 = 7;
const int DATA_3 = 6;
const int DATA_4 = 5;
const int DATA_5 = 4;
const int DATA_6 = 9;
const int DATA_7 = 2;
const int WE_PIN = 11;
const int CLOCK_PIN = 3;
const int BTN_PIN = 12;

int idx = 0;
bool pushed = false;

int count = 0;
unsigned long previousMillis = 0;

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
  pinMode(LED_BUILTIN, OUTPUT);

  /*TCCR2A = ((1 << WGM21) | (1 << COM2A0));
    TCCR2B = (1 << CS20); // prescaler = 1
    TIMSK2 = 0;
    OCR2A = 1;*/

  /*TCCR1A = ( (1 << COM1A0));
    TCCR1B = ((1 << WGM12) | (1 << CS10));
    TIMSK1 = 0;
    OCR1A = 0; // 0, 1, 3, 7 (8 MHz, 4 MHz, 2 MHz, 1 MHz)
    // 1: 250 kHz?
    // 0: 500 kHz?*/

  TCCR2A = 0x23;
  TCCR2B = 0x09;
  OCR2A = 2;
  OCR2B = 1;

  Serial.begin(115200);

  SendByte(0b10010000);
  SendByte(0b10111111);
  SendByte(0b11011111);
  SendByte(0b11111111);
}

void loop() {

  /*bool prevPushed = pushed;

    if (digitalRead(BTN_PIN) == HIGH) {
    pushed = true;
    } else pushed = false;

    if (pushed != prevPushed) idx++;

    if (idx % 4 == 0) {
    digitalWrite(LED_BUILTIN, 1);
    }
    if ((idx+2) % 4 == 0) {
    digitalWrite(LED_BUILTIN, 0);
    }*/

  //SendByte(0x83); SendByte(0x12); // sets channel 0 tone to 0x123
  //SendByte(0b10000011); SendByte(0b00010010);
  /*SendByte(0x9F); // sets channel 0 to loudest possible
    SendByte(0x9D); // sets channel 0 to loudest possible
    SendByte(0x9B); // sets channel 0 to loudest possible
    SendByte(0x99); // sets channel 0 to loudest possible
    SendByte(0x97); // sets channel 0 to loudest possible
    SendByte(0x95); // sets channel 0 to loudest possible
    SendByte(0x93); // sets channel 0 to loudest possible
    SendByte(0x91); // sets channel 0 to loudest possible*/
  //SendByte(0x90); // sets channel 0 to loudest possible
  //SendByte(0x9F); // sets channel 0 to lowest volume (silencing it)

  unsigned long currentMillis = millis();

  bool change = false;

  if (currentMillis - previousMillis >= 1000) {
    previousMillis = currentMillis;
    count++;
    change = true;
  }
  
  SendByte(0b10010000);

  /*Serial.println("0000000000");
    SendByte(0b10000000);
    SendByte(0b00000000);
    delay(500);*/
  /*Serial.println("0000000001");
    SendByte(0b10000000);
    SendByte(0b00000001);
    delay(500);
    Serial.println("0000000010");
    SendByte(0b10000000);
    SendByte(0b00000010);
    delay(500);
    Serial.println("0000000011");
    SendByte(0b10000000);
    SendByte(0b00000011);
    delay(500);
    Serial.println("0000000100");
    SendByte(0b10000000);
    SendByte(0b00000100);
    delay(500);
    Serial.println("0000000101");
    SendByte(0b10000000);
    SendByte(0b00000101);
    delay(500);
    Serial.println("0000000110");
    SendByte(0b10000000);
    SendByte(0b00000110);
    delay(500);
    Serial.println("0000000111");
    SendByte(0b10000000);
    SendByte(0b00000111);
    delay(500);

    Serial.println("0000001000");
    SendByte(0b10000000);
    SendByte(0b00001000);
    delay(500);
    Serial.println("0000001001");
    SendByte(0b10000000);
    SendByte(0b00001001);
    delay(500);
    Serial.println("0000001010");
    SendByte(0b10000000);
    SendByte(0b00001010);
    delay(500);
    Serial.println("0000001011");
    SendByte(0b10000000);
    SendByte(0b00001011);
    delay(500);
    Serial.println("0000001100");
    SendByte(0b10000000);
    SendByte(0b00001100);
    delay(500);
    Serial.println("0000001101");
    SendByte(0b10000000);
    SendByte(0b00001101);
    delay(10000);
    Serial.println("0000001110");
    SendByte(0b10000000);
    SendByte(0b00001110);
    delay(500);
    Serial.println("0000001111");
    SendByte(0b10000000);
    SendByte(0b00001111);
    delay(500);

    Serial.println("0000010000");
    SendByte(0b10000000);
    SendByte(0b00010000);
    delay(500);
    Serial.println("0000010001");
    SendByte(0b10000000);
    SendByte(0b00010001);
    delay(500);
    Serial.println("0000010010");
    SendByte(0b10000000);
    SendByte(0b00010010);
    delay(500);
    Serial.println("0000010011");
    SendByte(0b10000000);
    SendByte(0b00010011);
    delay(500);
    Serial.println("0000010100");
    SendByte(0b10000000);
    SendByte(0b00010100);
    delay(500);
    Serial.println("0000010101");
    SendByte(0b10000000);
    SendByte(0b00010101);
    delay(500);
    Serial.println("0000010110");
    SendByte(0b10000000);
    SendByte(0b00010110);
    delay(500);
    Serial.println("0000010111");
    SendByte(0b10000000);
    SendByte(0b00010111);
    delay(500);

    Serial.println("0000011000");
    SendByte(0b10000000);
    SendByte(0b00011000);
    delay(500);
    Serial.println("0000011001");
    SendByte(0b10000000);
    SendByte(0b00011001);
    delay(500);
    Serial.println("0000011010");
    SendByte(0b10000000);
    SendByte(0b00011010);
    delay(500);
    Serial.println("0000011011");
    SendByte(0b10000000);
    SendByte(0b00011011);
    delay(500);
    Serial.println("0000011100");
    SendByte(0b10000000);
    SendByte(0b00011100);
    delay(500);
    Serial.println("0000011101");
    SendByte(0b10000000);
    SendByte(0b00011101);
    delay(500);
    Serial.println("0000011110");
    SendByte(0b10000000);
    SendByte(0b00011110);
    delay(500);
    Serial.println("0000011111");
    SendByte(0b10000000);
    SendByte(0b00011111);
    delay(500);*/

  //first: 261 hz
  SendByte(0b10000000);
  SendByte(0b00101000);
  delay(5000);

  //second: 325 hz
  SendByte(0b10000000);
  SendByte(0b00100000);
  delay(300);
  
  //first: 261 hz
  SendByte(0b10000000);
  SendByte(0b00101000);
  delay(300);

  //second: 325 hz
  SendByte(0b10000000);
  SendByte(0b00100000);
  delay(300);

  //third: 387 hz
  SendByte(0b10000000);
  SendByte(0b00011011);
  delay(200);
  
  // fade out
  SendByte(0b10010011);
  delay(100);
  SendByte(0b10010111);
  delay(100);
  SendByte(0b10011011);
  delay(100);

  // fade to max
  SendByte(0b10010011);
  
  //third: 387 hz
  SendByte(0b10000000);
  SendByte(0b00011011);
  delay(200);

  // fade out
  SendByte(0b10010011);
  delay(100);
  SendByte(0b10010111);
  delay(100);
  SendByte(0b10011011);
  delay(100);
  
  /*SendByte(0b10000000);
  SendByte(0b00101000);
  delay(500);*/
  /*SendByte(0b10000010);
  SendByte(0b00010101);
  delay(300);
  SendByte(0b10000000);
  SendByte(0b00101111);
  delay(300);
  SendByte(0b10000000);
  SendByte(0b00101000);
  delay(500);
  SendByte(0b10000000);
  SendByte(0b00101000);
  delay(500);*/

  
  /*SendByte(0b10000010);
  SendByte(0b00010101);
  delay(300);
  SendByte(0b10000001);
  SendByte(0b00110111);
  delay(300);
  SendByte(0b10000001);
  SendByte(0b00100100);
  delay(600);
  SendByte(0b10000001);
  SendByte(0b00100100);
  delay(600);

  SendByte(0b10000010);
  SendByte(0b00010101);
  delay(300);
  SendByte(0b10000001);
  SendByte(0b00110111);
  delay(300);
  SendByte(0b10000010);
  SendByte(0b00010101);
  delay(300);
  SendByte(0b10000001);
  SendByte(0b00110111);
  delay(300);
  SendByte(0b10000001);
  SendByte(0b00100100);
  delay(600);
  SendByte(0b10000001);
  SendByte(0b00100100);
  delay(600);

  SendByte(0b10000001);
  SendByte(0b00001011);
  delay(300);
  SendByte(0b10000001);
  SendByte(0b00001111);
  delay(300);
  SendByte(0b10000001);
  SendByte(0b00011000);
  delay(300);
  SendByte(0b10000001);
  SendByte(0b00100100);
  delay(300);
  SendByte(0b10000001);
  SendByte(0b00110000);
  delay(600);
  SendByte(0b10000001);
  SendByte(0b00011000);
  delay(600);

  SendByte(0b10000001);
  SendByte(0b00100100);
  delay(300);
  SendByte(0b10000001);
  SendByte(0b00110000);
  delay(300);
  SendByte(0b10000001);
  SendByte(0b00110111);
  delay(300);
  SendByte(0b10000010);
  SendByte(0b00000101);
  delay(300);
  SendByte(0b10000010);
  SendByte(0b00010101);
  delay(600);
  SendByte(0b10000010);
  SendByte(0b00010101);
  delay(600);*/

  if (change) {
    if (count % 2 == 0) {
      //Serial.println("1");
      //SendByte(0b10010000);
      //SendByte(0b10110000);
      //SendByte(0b11010000);
      //SendByte(0b11110000);

      //SendByte(0b10011111);
      //SendByte(0b10111111);
      //SendByte(0b11011111);
      //SendByte(0b11111111);
    } else if ((count + 1) % 2 == 0) {
      //Serial.println("2");
      //SendByte(0b10010000);
      //SendByte(0b10110000);
      //SendByte(0b11010000);
      //SendByte(0b11110000);
    }
    change = false;
  }
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
  delay(10);
  digitalWrite(WE_PIN, HIGH);
  delay(10);
}
