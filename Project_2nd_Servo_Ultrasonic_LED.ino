#include <Servo.h>

//servo모터
const int SERVO = 10;
Servo servo;

//초음파 센서
const int trig_pin = 11;
const int echo_pin = 12;

// 뚜껑 열리는 속도 설정
int cnt = 0;

void setup() {
  
  //초음파 센서 셋업
  pinMode(trig_pin, OUTPUT);
  pinMode(echo_pin, INPUT);

  //servo모터 셋업
  servo.attach(SERVO);
  servo.write(0);
  delay(50);

  Serial.begin(115200);
}

void loop() {

  //초음파 측정 시작
  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);

  long duration = pulseIn(echo_pin, HIGH);
  long distance = ((duration * 0.0343) / 2);

  Serial.print(distance);
  Serial.println( " cm");
  delay(100);

  //servo모터

  if(distance <= 15){
    if(cnt >= 10){
      servo.write(140);
    }else{
      cnt = cnt + 3;
    }

  }else{
    if(cnt == 0){
      servo.write(90);
    }else{
      cnt--;
    }
  }
  delay(100);
}
