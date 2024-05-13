#define LED_SIZE 8 

const int LED[8] = {4, 5, 6, 7, 8, 9, 10, 11};

void setup()
{
  for(int i=0; i<LED_SIZE; i++){
    pinMode(LED[i], OUTPUT);
  }
}

// void loop()
// {
//   for(int t_high=0; high<= 10; t_high++)
//   {
//     analogWrite(LED, t_high*25);
//     delay(100);
//   }
// }

// 배열 이용하여 LED 불켜기
// const int LED[8]= {4,5,6,7,8,9,10,11};

// void setup() {
//   for(int i =0; i <8; i++)
//   {
//     pinMode(LED[i], OUTPUT);
//   }
// }

// void loop() {
//     for(int i =0; i <8; i++)
//   {
//     digitalWrite(LED[i], HIGH);
//   }
// }

//*모든 핀이 PWM 이 되는것은 아님! (숫자 앞에 '~'붙은것만 PWM 기능으로 직류성분을 만들어 주는것)
// 기존에 병근이가 해두었던 코드
void loop() {
  for(int t_high=0; t_high<=1500; t_high++){
    for(int i=0; i<LED_SIZE; ++i){
      digitalWrite(LED[i], HIGH);
    }
    delayMicroseconds(t_high);
    
    for(int i=0; i<LED_SIZE; i++){
      digitalWrite(LED[i], LOW);
    }
    delayMicroseconds(1500-t_high);
  }
}
