#include <Arduino.h>

#define INPUT_PIN 2

//bit values
#define SKIP 2
#define DATA 1
#define AXIS 0

#define OFFSET -5
#define PULSECALC(x) ((x - 2501)/500)
#define PREAMBLEMASK 0x3FFFF

#define X_FIELDS \
  X(uint16_t, fw_version)\
  X(uint32_t, ID)\
  X(uint16_t, fcal0Phase)\
  X(uint16_t, fcal1Phase)\
  X(uint16_t, fcal0Tilt)\
  X(uint16_t, fcal1Tilt)\
  X(uint8_t, unlockCount)\
  X(uint8_t, hwVersion)\
  X(uint16_t, fcal0Curve)\
  X(uint16_t, fcal1Curve)\
  X(int8_t, dirX)\
  X(int8_t, dirY)\
  X(int8_t, dirZ)\
  X(uint16_t, fcal0GibPhase)\
  X(uint16_t, fcal1GibPhase)\
  X(uint16_t, fcal0GibMag)\
  X(uint16_t, fcal1GibMag)\
  X(uint8_t, mode)\
  X(uint8_t, faults)

typedef struct OOTX {
  #define X(type, name) type name;
    X_FIELDS
  #undef X
} OOTX;

OOTX OOTX0, OOTX1;
unsigned long frame0 = 0xFFFFFFFF;
unsigned long frame1 = 0XFFFFFFFF;
unsigned int flag0 = 0;
unsigned int flag1 = 0;

void setup() {
  pinMode(INPUT_PIN, INPUT);
  Serial.begin(115200);
}

void loop() {
  signed long pulse0, pulse1;
  unsigned int value0, value1, dataBit0, dataBit1;
  pulse0 = (pulseIn(INPUT_PIN, LOW, 8000) + OFFSET) * 48;
  if (pulse0 > 1200) {
    pulse1 = (pulseIn(INPUT_PIN, LOW, 1000) + OFFSET) * 48;
    if (pulse1 > 1200) {
      value0 = PULSECALC(pulse0);
      dataBit0 = (value0 >> DATA) & B001;
      frame0 = (frame0 << 1) | dataBit0;
      if ((frame0 & PREAMBLEMASK) == B1) {
        Serial.println("Premable 0 Received!");
        flag0 = 1;
      }
      /*
        Serial.print("(");
        Serial.print(value0, BIN);
        Serial.print(", " );
        Serial.print(dataBit);
        Serial.print(") ");
        Serial.println(value1, BIN);
      */
      /*
        Serial.print(pulse0);
        Serial.print(", ");
        Serial.println(pulse1);
        delay(500);
      */
      /*
        if(!dataBit) {
        preambleCounter++;
        }
        frame0 = (frame0 << 1) | dataBit;
        //Serial.println(frame0 & PREAMBLEMASK, BIN);

        if(preambleCounter >= 17 && dataBit) {
        Serial.println("preamble counter tripped!");
        Serial.println(frame0 & PREAMBLEMASK, BIN);
        preambleCounter = 0;
        }

        if(dataBit) {
        preambleCounter = 0;
        }
      */
    }
  }
}
