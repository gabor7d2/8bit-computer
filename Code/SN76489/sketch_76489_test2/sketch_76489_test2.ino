// SN76489 example.
// Plays Chuckie-Egg death music.
// My basis for a novelty doorbell (mostly to annoy the wife).
// Kris Adcock, 23rd January 2014

#include <avr/pgmspace.h>

const int kPin_D0 = 5;
const int kPin_D1 = 6;
const int kPin_D2 = 7;
const int kPin_D3 = 8;
const int kPin_D4 = 9;
const int kPin_D5 = 10;
const int kPin_D6 = 11;
const int kPin_D7 = 12;
const int kPin_NotWE = 4;
const int CLOCK_PIN = 3;

// Data to send consists of amount of time to delay before sending, and the byte.
// The code that generated the array below puts two bytes on the same line if they'll be
// used by the SN76489 together to set a tone. "C0" refers to the channel number - this
// particular jingle only uses channel 0.
const PROGMEM uint8_t g_SoundData[] =
{
  0,
  0b10000000, 0, 0b00100011, 0,
  0x90, 6,
  0x92, 1,
  0x94, 1,
  0x96, 1,
  0x98, 1,
  0b10000000, 0, 0b00100011, 0,
  0x90, 6,
  0x92, 1,
  0x94, 1,
  0x96, 1,
  0x98, 1,
  0b10000000, 0, 0b00010101, 0,
  0x90, 12,
  0x92, 2,
  0x94, 2,
  0x96, 2,
  0x98, 2,
  0b10000000, 0, 0b00011000, 0,
  0x90, 12,
  0x92, 2,
  0x94, 2,
  0x96, 2,
  0x98, 2,
  0xFF, 20,
  0b10000000, 0, 0b00100011, 0,
  0x90, 6,
  0x92, 1,
  0x94, 1,
  0x96, 1,
  0x98, 1,
  0b10000000, 0, 0b00100011, 0,
  0x90, 6,
  0x92, 1,
  0x94, 1,
  0x96, 1,
  0x98, 1,
  0b10000000, 0, 0b00010101, 0,
  0x90, 12,
  0x92, 2,
  0x94, 2,
  0x96, 2,
  0x98, 2,
  0b10000000, 0, 0b00011000, 0,
  0x90, 12,
  0x92, 2,
  0x94, 2,
  0x96, 2,
  0x98, 2,
  0xFF, 20,
  0b10000000, 0, 0b00011011, 0,
  0x90, 12,
  0x92, 2,
  0x94, 2,
  0x96, 2,
  0x98, 2,
  0b10000000, 0, 0b00011100, 0,
  0x90, 12,
  0x92, 2,
  0x94, 2,
  0x96, 2,
  0x98, 2,
  0b10000000, 0, 0b00100000, 0,
  0x90, 6,
  0x92, 1,
  0x94, 1,
  0x96, 1,
  0x98, 1,
  0b10000000, 0, 0b00100011, 0,
  0x90, 6,
  0x92, 1,
  0x94, 1,
  0x96, 1,
  0x98, 1,
  0b10000000, 0, 0b00100000, 0,
  0x90, 6,
  0x92, 1,
  0x94, 1,
  0x96, 1,
  0x98, 1,
  0b10000000, 0, 0b00011100, 0,
  0x90, 6,
  0x92, 1,
  0x94, 1,
  0x96, 1,
  0x98, 1,
  0b10000000, 0, 0b00011011, 0,
  0x90, 12,
  0x92, 2,
  0x94, 2,
  0x96, 2,
  0x98, 2,
  0b10000000, 0, 0b00011100, 0,
  0x90, 12,
  0x92, 2,
  0x94, 2,
  0x96, 2,
  0x98, 2,
  0b10000000, 0, 0b00100000, 0,
  0x90, 6,
  0x92, 1,
  0x94, 1,
  0x96, 1,
  0x98, 1,
  0b10000000, 0, 0b00100011, 0,
  0x90, 6,
  0x92, 1,
  0x94, 1,
  0x96, 1,
  0x98, 1,
  0b10000000, 0, 0b00100000, 0,
  0x90, 12,
  0x92, 2,
  0x94, 2,
  0x96, 2,
  0x98, 2,
  0b10000000, 0, 0b00100011, 0,
  0x90, 12,
  0x92, 2,
  0x94, 2,
  0x96, 2,
  0x98, 2,
  0x9F
  
  
  
       /*0, 0x84, 0, 0x27, // C0 tone 0x274 200
       0, 0x90, // C0 vol 15
       5, 0x91, // C0 vol 14
       4, 0x92, // C0 vol 13
       4, 0x93, // C0 vol 12
       6, 0x8f, 0, 0x22, // C0 tone 0x22f 223
       0, 0x90, // C0 vol 15
       4, 0x91, // C0 vol 14
       4, 0x92, // C0 vol 13
       2, 0x84, 0, 0x27, // C0 tone 0x274 200
       0, 0x90, // C0 vol 15
       4, 0x91, // C0 vol 14
       4, 0x92, // C0 vol 13
       4, 0x93, // C0 vol 12
       8, 0x80, 0, 0x2c, // C0 tone 0x2c0 891
       0, 0x90, // C0 vol 15
       4, 0x91, // C0 vol 14
       4, 0x92, // C0 vol 13
       2, 0x8b, 0, 0x2e, // C0 tone 0x2eb 167
       0, 0x90, // C0 vol 15
       4, 0x91, // C0 vol 14
       4, 0x92, // C0 vol 13
       4, 0x93, // C0 vol 12
       8, 0x8c, 0, 0x3a, // C0 tone 0x3ac 398
       0, 0x90, // C0 vol 15
       4, 0x91, // C0 vol 14
       4, 0x92, // C0 vol 13
       2, 0x85, 0, 0x34, // C0 tone 0x345 445
       0, 0x90, // C0 vol 15
       4, 0x91, // C0 vol 14
       4, 0x92, // C0 vol 13
       4, 0x93, // C0 vol 12
       8, 0x84, 0, 0x3e, // C0 tone 0x3e4 126
       0, 0x90, // C0 vol 15
       4, 0x91, // C0 vol 14
       4, 0x92, // C0 vol 13
       2, 0x8c, 0, 0x3a, // C0 tone 0x3ac 398
       0, 0x90, // C0 vol 15
       4, 0x91, // C0 vol 14
       4, 0x92, // C0 vol 13
       4, 0x93, // C0 vol 12
      48, 0x90, // C0 vol 15
       4, 0x91, // C0 vol 14
       1, 0x85, 0, 0x34, // C0 tone 0x345
       0, 0x90, // C0 vol 15
       4, 0x91, // C0 vol 14
       1, 0x8b, 0, 0x2e, // C0 tone 0x2eb
       0, 0x90, // C0 vol 15
       4, 0x91, // C0 vol 14
       1, 0x80, 0, 0x2c, // C0 tone 0x2c0
       0, 0x90, // C0 vol 15
       4, 0x91, // C0 vol 14
       1, 0x84, 0, 0x27, // C0 tone 0x274
       0, 0x90, // C0 vol 15
       4, 0x91, // C0 vol 14
       1, 0x83, 0, 0x1f, // C0 tone 0x1f3
       0, 0x90, // C0 vol 15
       4, 0x91, // C0 vol 14
       1, 0x87, 0, 0x1d, // C0 tone 0x1d7
       0, 0x90, // C0 vol 15
       4, 0x91, // C0 vol 14
       2, 0x92, // C0 vol 13
       2, 0x93, // C0 vol 12
       1, 0x94, // C0 vol 11
       2, 0x95, // C0 vol 10
       1, 0x96, // C0 vol 9
       2, 0x97, // C0 vol 8
       2, 0x98, // C0 vol 7
       1, 0x99, // C0 vol 6
       2, 0x9a, // C0 vol 5
       1, 0x9b, // C0 vol 4
       2, 0x9c, // C0 vol 3
       2, 0x9d, // C0 vol 2
       1, 0x9e, // C0 vol 1
       2, 0x9f, // C0 vol 0*/
       
// Done. 90 lines.
// Multiplier for time required = 10
};

const int kDataLength = 143; // number of commands in the data to send (in total)
const int kTimeMultiplier = 20;
// The time delays in the data have been divided down by a "safe" constant, so I can be sure they'll fit into a byte.
// So we need to multiply those values up when calculating how long to delay for.

int g_iLine = 0; // current line (command, really)
unsigned long g_LastTime = 0; // time of last command executed, in milliseconds

void setup()
{
  pinMode(kPin_D0, OUTPUT);
  pinMode(kPin_D1, OUTPUT);
  pinMode(kPin_D2, OUTPUT);
  pinMode(kPin_D3, OUTPUT);
  pinMode(kPin_D4, OUTPUT);
  pinMode(kPin_D5, OUTPUT);
  pinMode(kPin_D6, OUTPUT);
  pinMode(kPin_D7, OUTPUT);
  pinMode(kPin_NotWE, OUTPUT);
  
  pinMode(CLOCK_PIN, OUTPUT);
  TCCR2A = 0x23;
  TCCR2B = 0x09;
  OCR2A = 3;
  OCR2B = 1;

  digitalWrite(kPin_NotWE, HIGH);
  
  SilenceAllChannels();

  delay(500);
  
  g_iLine = 0;
  g_LastTime = millis();
}

void PutByte(byte b)
{
  digitalWrite(kPin_D0, (b&1)?HIGH:LOW);
  digitalWrite(kPin_D1, (b&2)?HIGH:LOW);
  digitalWrite(kPin_D2, (b&4)?HIGH:LOW);
  digitalWrite(kPin_D3, (b&8)?HIGH:LOW);
  digitalWrite(kPin_D4, (b&16)?HIGH:LOW);
  digitalWrite(kPin_D5, (b&32)?HIGH:LOW);
  digitalWrite(kPin_D6, (b&64)?HIGH:LOW);
  digitalWrite(kPin_D7, (b&128)?HIGH:LOW);
}

void SendByte(byte b)
{
  digitalWrite(kPin_NotWE, HIGH);
  PutByte(b);
  digitalWrite(kPin_NotWE, LOW);
  delay(1);
  digitalWrite(kPin_NotWE, HIGH);
}

void SilenceAllChannels()
{
  SendByte(0x9f);
  SendByte(0xbf);
  SendByte(0xdf);
  SendByte(0xff);
}

void loop()
{
  if (g_iLine >= kDataLength)
  {
    // run out of data! So silence, pause, then restart.
    SilenceAllChannels();
    delay(2000);
    g_iLine = 0;
    g_LastTime = millis();
  } else
  {
    // still more data to play. So we check how long it has been since we last sent any bytes,
    // and compare that with the delay in the array.
    
    byte iDelay = pgm_read_byte_near(g_SoundData + (g_iLine << 1));
    byte iByte = pgm_read_byte_near(g_SoundData + (g_iLine << 1) + 1);
    unsigned int iTimeElapsed = millis() - g_LastTime; // how much delay has elapsed so far
    
    // now we play all the data that is due
    while ((iDelay * kTimeMultiplier) <= iTimeElapsed && g_iLine < kDataLength)
    {
      g_LastTime = millis();
      
      // send this byte ...
      SendByte(iByte);
      
      // increment line count and grab next byte ...
      g_iLine++;
      iDelay = pgm_read_byte_near(g_SoundData + (g_iLine << 1));
      iByte = pgm_read_byte_near(g_SoundData + (g_iLine << 1) + 1);
      iTimeElapsed = millis() - g_LastTime;
    }
  }
}
