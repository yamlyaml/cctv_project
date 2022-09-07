#include "pitches.h"
//Buzzer 작동과 관련된 코드입니다.

//매크로 상수로 음표 박자와 쉼표 박자를 설정합니다. 현재 값은 1000입니다.
#define BEAT 1000
#define REST_BEAT 1000

int Buzzer1_pin = 10;
int Buzzer2_pin = 9;
int Buzzer3_pin = 8;
int notes[] = {NOTE_C4,NOTE_E4,NOTE_C5,NOTE_E5,NOTE_C6,NOTE_E6};

//millis 구현을 위한 변수입니다. 여기서 interval 은 쉼표 길이로 합니다.
unsigned long Buzzer1_previousMillis = 0;
unsigned long Buzzer1_interval = REST_BEAT;

unsigned long Buzzer2_previousMillis = 0;
unsigned long Buzzer2_interval = REST_BEAT;

unsigned long Buzzer3_previousMillis = 0;
unsigned long Buzzer3_interval = REST_BEAT;

// 부저 초기화입니다. 테스트로 음을 두번 출력합니다.
void Buzzer_setup(){
  Serial.println("Buzzer activated");
  tone(Buzzer1_pin,notes[0],500);
  delay(1000);
  tone(Buzzer2_pin,notes[2],500);
  delay(1000);
  tone(Buzzer3_pin,notes[4],500);
  delay(1000);
  
  }

void Buzzer1_loop(){
  unsigned long currentMillis = millis();
  if(currentMillis - Buzzer1_previousMillis >= Buzzer1_interval){
    Buzzer1_previousMillis = currentMillis;

//  음을 출력하고 delay로 음을 유지하도록 합니다. noTone함수는 음을 끊어줍니다.
    tone(Buzzer1_pin,notes[0],BEAT);
    delay(1000);
    noTone(Buzzer1_pin);
    tone(Buzzer1_pin,notes[1],BEAT);
    delay(1000);
    noTone(Buzzer1_pin);
    
    }
  }

void Buzzer2_loop(){
  unsigned long currentMillis = millis();
  if(currentMillis - Buzzer2_previousMillis >= Buzzer2_interval){
    Buzzer2_previousMillis = currentMillis;

//  음을 출력하고 delay로 음을 유지하도록 합니다. noTone함수는 음을 끊어줍니다.
    tone(Buzzer2_pin,notes[2],BEAT);
    delay(1000);
    noTone(Buzzer2_pin);
    tone(Buzzer2_pin,notes[3],BEAT);
    delay(1000);
    noTone(Buzzer2_pin);
    }
  }

void Buzzer3_loop(){
  unsigned long currentMillis = millis();
  if(currentMillis - Buzzer3_previousMillis >= Buzzer3_interval){
    Buzzer3_previousMillis = currentMillis;

//  음을 출력하고 delay로 음을 유지하도록 합니다. noTone함수는 음을 끊어줍니다.
    tone(Buzzer3_pin,notes[4],BEAT);
    delay(1000);
    noTone(Buzzer3_pin);
    tone(Buzzer3_pin,notes[5],BEAT);
    delay(1000);
    noTone(Buzzer3_pin);
    
    }
  }
