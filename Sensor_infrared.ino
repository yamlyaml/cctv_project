int Sensor_Infrared1_Pin = A13;
int Sensor_Infrared1_Value = 0;
int Sensor_Infrared1_mappedValue = 0;
int Sensor_Output1 = 0;
unsigned long Sensor_Infrared1_previousMillis = 0;
unsigned long Sensor_Infrared1_interval = 1000;

int Sensor_Infrared2_Pin = A14;
int Sensor_Infrared2_Value = 0;
int Sensor_Infrared2_mappedValue = 0;
int Sensor_Output2 = 0;
unsigned long Sensor_Infrared2_previousMillis = 0;
unsigned long Sensor_Infrared2_interval = 1000;

int Sensor_Infrared3_Pin = A15;
int Sensor_Infrared3_Value = 0;
int Sensor_Infrared3_mappedValue = 0;
int Sensor_Output3 = 0;
unsigned long Sensor_Infrared3_previousMillis = 0;
unsigned long Sensor_Infrared3_interval = 1000;

unsigned long Sensor_detect1_previousMillis = 0;
unsigned long Sensor_detect2_previousMillis = 0;
unsigned long Sensor_detect3_previousMillis = 0;
unsigned long Sensor_detect_interval = 1000;
int Sensor1_data = 0;
int Sensor2_data = 0;
int Sensor3_data = 0;
int cnt = 0;
int cnt2 = 0;
int cnt3 = 0;



// 적외선 센서 초기화함수는Sensor_Output 의 포인터를 반환합니다.
void Sensor_Infrared1_setup(){
  Serial.println("Infrared Sensor1 activated");
}

void Sensor_Infrared1_loop(){
  unsigned long currentMillis = millis();

  if(currentMillis - Sensor_Infrared1_previousMillis >= Sensor_Infrared1_interval){
    Sensor_Infrared1_previousMillis = currentMillis;

    Sensor_Infrared1_Value = analogRead(Sensor_Infrared1_Pin);
    Sensor_Infrared1_mappedValue = map(Sensor_Infrared1_Value,0,1023,0,255);
    
// Sensor Value 확인 코드
//    Serial.print(Sensor_Infrared1_Value);
//    Serial.print("-------");
//    Serial.println(Sensor_Infrared1_mappedValue);
//    delay(1000);


//  센서 신호를 맵핑한 값이 특정값을 넘으면 1을 반환합니다. 특정 거리에 들어왔을 때 감지 신호를 보내기 위한 코드입니다.
    if(Sensor_Infrared1_mappedValue > 80){//차량 감지시
      Sensor_Output1 = 1;//
      Rfid1_loop();
      Sensor1_case();
    } 
      
    else{//차량이 없을때 모든 센서데이터 초기화
      Sensor_Output1 = 0;
      Rfid_Output1 = 0;
      Sensor1_data = 0;
      cnt = 0;
    }
  }
}


void Sensor1_case(){//3구역 빨강
  cnt++;
  if(Sensor_Output1 == 0){
      Sensor1_data = 0;
  }
  else if(Rfid_Output1 == 1){
      Sensor1_data = 2;//지정주차
  }
  else if((cnt > 5)&&(Rfid_Output1 == 0)){
      Sensor1_data = 1;//대기시간 이후 불법주차로 판단
  }
  else{
      Sensor1_data = 0;
  }
}

void Sensor_Infrared2_setup(){
  Serial.println("Infrared Sensor2 activated");
}

void Sensor_Infrared2_loop(){
  unsigned long currentMillis = millis();

  if(currentMillis - Sensor_Infrared2_previousMillis >= Sensor_Infrared2_interval){
    Sensor_Infrared2_previousMillis = currentMillis;

    Sensor_Infrared2_Value = analogRead(Sensor_Infrared2_Pin);
    Sensor_Infrared2_mappedValue = map(Sensor_Infrared2_Value,0,1023,0,255);
    
// Sensor Value 확인 코드
//    Serial.print(Sensor_Infrared2_Value);
//    Serial.print("-------");
//    Serial.println(Sensor_Infrared2_mappedValue);
//    delay(1000);


//  센서 신호를 맵핑한 값이 특정값을 넘으면 1을 반환합니다. 특정 거리에 들어왔을 때 감지 신호를 보내기 위한 코드입니다.
    if(Sensor_Infrared2_mappedValue > 80){//차량 감지시
      Sensor_Output2 = 1;//
      Rfid2_loop();
      Sensor2_case();
    } 
      
    else{//차량이 없을때 모든 센서데이터 초기화
      Sensor_Output2 = 0;
      Rfid_Output2 = 0;
      Sensor2_data = 0;
      cnt2 = 0;
    }
  }
}


void Sensor2_case(){//2구역
  cnt2++;
  if(Sensor_Output2 == 0){
      Sensor2_data = 0;
  }
  else if(Rfid_Output2 == 1){
      Sensor2_data = 8;
  }
  else if((cnt2 > 5)&&(Rfid_Output2 == 0)){
      Sensor2_data = 4;
  }
  else{
      Sensor2_data = 0;
  }
}

void Sensor_Infrared3_setup(){
  Serial.println("Infrared Sensor3 activated");
}

void Sensor_Infrared3_loop(){
  unsigned long currentMillis = millis();

  if(currentMillis - Sensor_Infrared3_previousMillis >= Sensor_Infrared3_interval){
    Sensor_Infrared3_previousMillis = currentMillis;

    Sensor_Infrared3_Value = analogRead(Sensor_Infrared3_Pin);
    Sensor_Infrared3_mappedValue = map(Sensor_Infrared3_Value,0,1023,0,255);
    
// Sensor Value 확인 코드
//    Serial.print(Sensor_Infrared3_Value);
//    Serial.print("-------");
//    Serial.println(Sensor_Infrared3_mappedValue);
//    delay(1000);


//  센서 신호를 맵핑한 값이 특정값을 넘으면 1을 반환합니다. 특정 거리에 들어왔을 때 감지 신호를 보내기 위한 코드입니다.
    if(Sensor_Infrared3_mappedValue > 80){//차량 감지시
      Sensor_Output3 = 1;//
      Rfid3_loop();
      Sensor3_case();
    } 
      
    else{//차량이 없을때 모든 센서데이터 초기화
      Sensor_Output3 = 0;
      Rfid_Output3 = 0;
      Sensor3_data = 0;
      cnt3 = 0;
    }
  }
}


void Sensor3_case(){//1구역 주황
  cnt3++;
  if(Sensor_Output3 == 0){
      Sensor2_data = 0;
  }
  else if(Rfid_Output3 == 1){
      Sensor3_data = 32;
  }
  else if((cnt3 > 5)&&(Rfid_Output3 == 0)){
      Sensor3_data = 16;
  }
  else{
      Sensor3_data = 0;
  }
}
