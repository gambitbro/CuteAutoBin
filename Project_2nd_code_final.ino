// ULTRASONIC, SERVO, LED, LCD, millis() 추가

#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


// 초음파 센서(뚜껑 개폐)
#define TRIG_PIN 11
#define ECHO_PIN 12
// 초음파 센서(쓰레기양 측정)
#define TRIG_PIN2 9
#define ECHO_PIN2 10
// LED
#define TRIG_LED 6
//쓰레기통 높이
#define BINHEIGHT 25
// Buzzer음 개수
#define NOTE_SIZE 5

// Servo 모터
const int SERVO = 3;
Servo servo;

// 뚜껑 열리는 속도 설정
int cnt = 0;
// LED의 밝기를 저장하는 변수
int brightness = 0; 
// LCD 설정
LiquidCrystal_I2C lcd(0x27,16,2);


// millis 설정
unsigned long pre_t1 = 0;
unsigned long pre_t2 = 0;

const long interval_t1 = 1000;  //1초간격
const long interval_t2 = 500; //0.5초간격

//BUZZER
const int BUZZER = 5;
double melody[NOTE_SIZE] = {261.6, 329.6, 392.0, 329.6, 261.6};

//==============================================================

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
  servo.write(90);    //닫혀있는 상태로 초기화
  delay(50);

  // LED 셋업
  pinMode(trig_led, OUTPUT);
  digitalWrite(trig_led, LOW);

  Serial.begin(115200);    //초음파 거리 모니터링
}

//==============================================================

void loop() {

  unsigned long curr_t1 = millis();
  unsigned long curr_t2 = millis();  //예비

  // 쓰레기양 초음파 millis 1번
  if(curr_t1 - pre_t1 >= interval_t1){
    pre_t1 = curr_t1;
    // 작업 시작
    // 쓰레기양 초음파 측정 시작
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

    //  LCD에 표시할 메시지 결정
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
      for(int note = 0; note < NOTE_SIZE; ++note){
        tone(BUZZER, melody[note]);
        delay(50);
        noTone(BUZZER);
      }
    }
    // LCD에 메시지 출력
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Bin Status:");
    lcd.setCursor(0, 1);
    lcd.print(message);

    //delay(50);
  }

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

  // Servo 모터 및 LED 제어
  if (distance <= 15) {
    if (cnt >= 10) {
      servo.write(130);
      for(int t_high=0; t_high<=5; t_high++)
      {
        analogWrite(trig_led, brightness); // LED 밝기 설정
        delay(20); // 깜빡이는 속도 느리게
        brightness += 25; // 밝기 증가
      }
      for(int t_low=0; t_low<=5; t_low++)
      {
        analogWrite(trig_led, brightness); // LED 밝기 설정
        delay(20); // 깜빡이는 속도 느리게
        brightness -= 25; // 밝기 감소
      }
    } else {
      cnt = cnt + 5;  // 15cm 이내 물체를 감지하면 cnt값을 5올림, 10이되면 뚜껑 오픈
    }
  } else {
    if (cnt == 0) {
      servo.write(90);
      digitalWrite(trig_led, 0); // LED 꺼짐
    } else {
      cnt--;    // 쓰레기 버리는 시간을 위해 올라갔던 cnt값이 하나씩 감소되도록 설정
    }
  }
  delay(100);
}
