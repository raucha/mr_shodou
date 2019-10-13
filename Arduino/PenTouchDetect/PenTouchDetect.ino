/*********************************************************
  This is a library for the MPR121 12-channel Capacitive touch sensor
  Designed specifically to work with the MPR121 Breakout in the Adafruit shop
  ----> https://www.adafruit.com/products/
  These sensors use I2C communicate, at least 2 pins are required
  to interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!
  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
**********************************************************/

#include <Wire.h>
#include "Adafruit_MPR121.h"

#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif

// You can have up to 4 on one i2c bus but one is enough for testing!
Adafruit_MPR121 cap = Adafruit_MPR121();

// uint16_t currtouched = 0;

void setup() {
  Serial.begin(57600);
  while (!Serial) {  // needed to keep leonardo/micro from starting too fast!
    delay(10);
  }
  Serial.println("Adafruit MPR121 Capacitive Touch sensor test");

  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1)
      ;
  }
  Serial.println("MPR121 found!");
}

void loop() {
  //  ループ周期まで待機
  static int prevMillis = 0;
  int currentMillis = millis();
  const int PERIOD_MS = 10;
  while (PERIOD_MS > millis() - prevMillis)
    ;
  prevMillis = millis();

  //  static int isPenPushed = 0;
  static int prevPenVal = 0;

  // 静電容量センサ 生値の取得
  // currtouched = cap.touched();
  int penVal_raw = cap.filteredData(6);

  // LPF
  static float penVal = penVal_raw;
  static float penVal2 = penVal_raw;
  float k = 0.99;
  penVal = k * ((float)penVal) + (1.0 - k) * ((float)penVal_raw);
  float k2 = 0.8;
  penVal2 = k2 * ((float)penVal2) + (1.0 - k2) * ((float)penVal_raw);

  // 変化速度を取得
  static float preDiff = penVal2 - penVal;
  float diff = penVal2 - penVal;
  float dDiff = diff - preDiff;
  preDiff = diff;
  static float prevDDiff = dDiff;

  const int EVENT_STAY = 0;
  const int EVENT_PUSH = 1;
  const int EVENT_RELEASE = 2;
  int penEvent = EVENT_STAY;
  //  const int release_th = 10;
  const float release_th = 3.0;
  if (-release_th > dDiff && -release_th < prevDDiff) {
    penEvent = EVENT_PUSH;
  } else if (release_th < dDiff && release_th > prevDDiff) {
    penEvent = EVENT_RELEASE;
  }
  //  if (release_th < abs(penVal2 - penVal)) {
  //    if (penVal2 < penVal) {
  //      penEvent = EVENT_PUSH;
  //    } else {
  //      penEvent = EVENT_RELEASE;
  //    }
  //  }
  prevPenVal = penVal;
  prevDDiff = dDiff;

      // pythonへのデータ送付用
      Serial.print("{");
      Serial.print(" \"state\":");
      Serial.print(penEvent);
      Serial.print(", \"val\":");
      Serial.print(penVal);
      Serial.println("}");

//  // シリアルプロット用
//  Serial.print(penVal_raw);
//  Serial.print(",");
//  Serial.print(penVal2);
//  Serial.print(",");
//  Serial.print(diff);
//  Serial.print(",");
//  Serial.print(dDiff);
//  Serial.print(",");
//  Serial.print(penVal);
//  Serial.println("");

  // comment out this line for detailed data from the sensor!
  return;

  //  // デバッグ用
  //  Serial.print("\t\t\t\t\t\t\t\t\t\t\t\t\t 0x");
  //  Serial.println(cap.touched(), HEX); Serial.print("Filt: "); for (uint8_t
  //  i=0; i<12; i++) {
  //    Serial.print(cap.filteredData(i)); Serial.print("\t");
  //  }
  //  Serial.println();
  //  Serial.print("Base: ");
  //  for (uint8_t i=0; i<12; i++) {
  //    Serial.print(cap.baselineData(i)); Serial.print("\t");
  //  }
  //  Serial.println();

  // put a delay so it isn't overwhelming
  delay(10);
}
