// LED 스트립 추가

#include <Servo.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <FastLED.h> // FastLED 라이브러리 추가

// Servo 모터
const int SERVO = 10;
Servo servo;

// 초음파 센서(뚜껑 개폐)
const int trig_pin = 11;
const int echo_pin = 12;
// 초음파 센서(쓰레기양 측정)
const int trig_pin2 = 5; // 쓰레기양 측정 초음파 센서 핀 *
const int echo_pin2 = 9; // 쓰레기양 측정 초음파 센서 핀 *

// LED 스트립
#define LED_STRIP_PIN 6 // LED 스트립이 연결된 핀
#define NUM_LEDS 60     // LED 스트립의 LED 개수
CRGB leds[NUM_LEDS];    // LED 스트립의 색상을 저장하는 배열

// 뚜껑 열리는 속도 설정
int cnt = 0;
// LED의 밝기를 저장하는 변수
int brightness = 0; 

LiquidCrystal_I2C lcd(0x27,16,2);

//쓰레기통 높이
const int binHeight = 22;

void setup() {
  //LCD 셋업
  lcd.init();

  // 초음파 센서 셋업
  pinMode(trig_pin, OUTPUT);
  pinMode(echo_pin, INPUT);
  pinMode(trig_pin2, OUTPUT); // 쓰레기양 측정 초음파 센서 핀 *
  pinMode(echo_pin2, INPUT); // 쓰레기양 측정 초음파 센서 핀 *
  lcd.begin(16, 2); // 16x2 LCD 초기화 *
  // Servo 모터 셋업
  servo.attach(SERVO);
  servo.write(0);
  delay(50);

  // LED 스트립 초기화
  FastLED.addLeds<WS2812, LED_STRIP_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
  FastLED.clear();
  FastLED.show();

  Serial.begin(115200);
}

void loop() {

  // 뚜껑열리는 초음파 측정 시작
  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);

  long duration = pulseIn(echo_pin, HIGH);
  long distance = ((duration * 0.0343) / 2);

  Serial.print(distance);
  Serial.println(" cm");
  delay(100);

  // 쓰레기양 초음파 측정 시작 *
  digitalWrite(trig_pin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trig_pin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin2, LOW);

  long duration2 = pulseIn(echo_pin2, HIGH);
  long distance2 = ((duration2 * 0.0343) / 2);

  Serial.print(distance2);
  Serial.println(" cm");
  delay(100);
  
  //  LCD에 표시할 메시지 결정 *
  String message;

  if (distance2 > binHeight * 0.9) {
    message = "EMPTY :)";
  } else if (distance2 > binHeight * 0.8) {
    message = "20%  O____";
  } else if (distance2 > binHeight * 0.6) {
    message = "40%  OO___";
  } else if (distance2 > binHeight * 0.4) {
    message = "60%  OOO__";
  } else if (distance2 > binHeight * 0.2) {
    message = "80%  OOOO_";
  } else {
    message = "FULL :(";
  }
  // LCD에 메시지 출력 *
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Bin Status:");
  lcd.setCursor(0, 1);
  lcd.print(message);

  delay(50); // 1초 대기

  // Servo 모터 및 LED 스트립 제어
  if (distance <= 15) {
    if (cnt >= 10) {
      servo.write(140);
      // LED 스트립 색상 변경 및 밝기 조절
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB(0, 255 - 4 * i, 4 * i);
        FastLED.setBrightness(2 * i);
      }
      FastLED.show();
      delay(150); // 각 단계의 지속 시간을 줄임

      // LED 스트립 색상 변경 및 밝기 조절
      for (int i = NUM_LEDS; i > 0; i--) {
        leds[i] = CRGB(4 * i, 0, 255 - 4 * i);
        FastLED.setBrightness(100 - i);
      }
      FastLED.show();
      delay(150); // 각 단계의 지속 시간을 줄임
    } else {
      cnt = cnt + 5;
    }
  } else {
    if (cnt == 0) {
      servo.write(90);
      FastLED.clear(); // LED 스트립 초기화
      FastLED.show();
    } else {
      cnt--;
    }
  }
  delay(100);
}
