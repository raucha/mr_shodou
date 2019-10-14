#include <Wire.h>
#include "Adafruit_MPR121.h"

#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif

// You can have up to 4 on one i2c bus but one is enough for testing!
Adafruit_MPR121 cap = Adafruit_MPR121();

// uint16_t currtouched = 0;

void setup() {
  pinMode(3, OUTPUT);
  // pinMode(5, OUTPUT);
  // pinMode(6, OUTPUT);
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
  float k2 = 0.90;
  penVal2 = k2 * ((float)penVal2) + (1.0 - k2) * ((float)penVal_raw);
  static float prevPenVal2 = penVal2;

  // 変化速度を取得
  static float prevDiff = penVal2 - penVal;
  float diff = penVal2 - penVal;
  float dDiff = diff - prevDiff;
  static float prevDDiff = dDiff;

  // 速度のLPFでの検出用
  static float prevRawVal = penVal_raw;
  float spd = prevPenVal - penVal_raw;
  static int spd_flted = spd;
  float k3 = 0.9;
  spd_flted = k3 * ((float)spd_flted) + (1.0 - k3) * ((float)spd);

  const int EVENT_STAY = 0;
  const int EVENT_PUSH = 1;
  const int EVENT_RELEASE = 2;
  int penEvent = EVENT_STAY;
  const int STATE_RELEASE = 0;
  const int STATE_PUSH = 1;
  // const int STATE_UNKOWN = 2;
  static int penState = STATE_RELEASE;
  static int pushedTime = millis();

  if (2000 < millis() - pushedTime) {
    penState = STATE_RELEASE;
  }

  //  const int release_th = 10;
  //  const float release_th = 3.0;
  //  const float release_th = 20.0;
  const float th_h = 120.0;
  const float th_l = 110.0;

  if (th_l > penVal2 && th_l < prevPenVal2 && STATE_RELEASE == penState) {
    //  if (-release_th > diff && -release_th < prevDiff && STATE_RELEASE ==
    //  penState) {
    // if (-release_th > dDiff && -release_th < prevDDiff) {
    // しきい値を跨いだ時に反応．ただし，近傍2秒間でプッシュされていて，その後リリースしていない場合は反応しない．
    penEvent = EVENT_PUSH;
    penState = STATE_PUSH;
    pushedTime = millis();
    digitalWrite(3, HIGH);
  } else if (th_h < penVal2 && th_h > prevPenVal2) {
    //  } else if (release_th < diff && release_th > prevDiff) {
    //  } else if (release_th < dDiff && release_th > prevDDiff) {
    penEvent = EVENT_RELEASE;
    penState = STATE_RELEASE;
    digitalWrite(3, LOW);
  }

  prevPenVal = penVal;
  prevDiff = diff;
  prevDDiff = dDiff;
  prevPenVal2 = penVal2;

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
  //      Serial.print(",");
  //    Serial.print(diff);
  //      Serial.print(",");
  //      Serial.print(dDiff);
  //      Serial.print(",");
  //      Serial.print(penVal);
  //  Serial.print(spd_flted);
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
