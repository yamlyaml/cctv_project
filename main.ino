#include <SoftwareSerial.h>
//SoftwareSerial.h 을 이용한 블루투스 통신 프로그램입니다.
//Bluetooth PW: 8243 (TEAMNULL) , 1234(HC-06)
// BluetoothModule 을 매크로 상수로 Serial1을 지정합니다. Serial1은 Arduino Mega 2560 보드의 18,19번 핀(TX1,RX1)을 통한 시리얼 통신 포트입니다. 
// 이 핀에 블루트스 모듈(HC-06)의 RX,TX핀과 교차로 연결하여 Serial1은 블루투스 모듈을 통해 데이터 통신을 하는 포트가 됩니다. 
#define BluetoothModule Serial1

//변수 선언부
//int Sensor_Infrared_enable = 0; : 적외선 센서 loop함수를 동작시키는 조건문의 변수입니다. 1은 on, 0은 off입니다.
//int Buzzer_enable = 0; : 부저 loop함수를 동작시키는 조건문의 변수입니다. 1은 on, 0은 off입니다.

int Sensor_Infrared_enable = 0;
int Buzzer_enable = 0;
int Output_Final = 0;


//블루투스 모듈을 통해 데이터를 보내기위한 함수입니다. loop함수의 주기와 독립적으로 데이터를 라즈베리 파이에 송신하기 위해 millis()함수를 이용했습니다.


unsigned long Signal_Send_previousMillis = 0;
unsigned long Signal_Send_interval = 1000;


void Signal_Send(){
  unsigned currentMillis = millis();
    if(currentMillis - Signal_Send_previousMillis >= Signal_Send_interval){
      Signal_Send_previousMillis = currentMillis;

//    센서의 set_up함수가 반환한 포인터를 이용해 다시 변수에 데이터를 담습니다. 
      Output_Final = Sensor1_data+Sensor2_data+Sensor3_data;

//    데이터를 담은 변수를 블루투스 모듈을 통해 송신합니다. 
      BluetoothModule.write(Output_Final);

//    보낸 데이터를 시리얼 모니터로 확인하기 위한 용도입니다. 
      Serial.println(Output_Final);
    }
  
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  BluetoothModule.begin(9600);
  Serial.println("Serial & Bluetooth connect start");

  Sensor_Infrared1_setup(); 
  Sensor_Infrared2_setup(); 
  Sensor_Infrared3_setup(); 
//  Buzzer 초기화로 테스트 음을 발생시키며 시리얼 모니터에 start문구를 출력합니다.
  Buzzer_setup();
  Rfid_setup();
  

}

void loop(){
//  적외선 센서 감지 결과를 블루투스 모듈을 통해 라즈베리 파이로 보냅니다. Signal_Send_interval 값에 의해 현재 1초(1000ms)마다 결과를 보냅니다. 적외선 센서 -> 아두이노 ->라즈베리
  Signal_Send();
  Sensor_Infrared1_loop();
  Sensor_Infrared2_loop();
  Sensor_Infrared3_loop();


//  라즈베리 파이로부터 데이터가 오면 이를 정수형으로 변환하여 Buzzer_enable변수에 담습니다. 라즈베리 파이에서 0을 보내면 부저 off,1/2/3을 보내면 buzzer1/2/3 on 입니다.
  if(BluetoothModule.available()){
    Buzzer_enable = BluetoothModule.parseInt();
    Serial.println(Buzzer_enable);
  }

//  Buzzer_enable의 값으로 Buzzer_loop 함수를 on/off합니다.
  if(Buzzer_enable == 1){
    Buzzer1_loop();
  }
  else if(Buzzer_enable == 2){
    Buzzer2_loop();
  }
  else if(Buzzer_enable == 3){
    Buzzer3_loop();
  }
}
