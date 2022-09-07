#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN   5     // reset핀 설정
#define SS_PIN1   53    //1번구역 데이터 송수신 포트
#define SS_PIN2   49
#define SS_PIN3   48
#define NR_OF_READERS   3
//MOSI 51 MISO 50 SCK 52 VCC 3.3V

byte ssPins[] = {SS_PIN1, SS_PIN2,SS_PIN3};

MFRC522 mfrc522[NR_OF_READERS];

int Rfid_Output1 = 0;
int Rfid_Output2 = 0;
int Rfid_Output3 = 0;

void Rfid_setup() {
  SPI.begin();                             // SPI 초기화

  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
    mfrc522[reader].PCD_Init(ssPins[reader], RST_PIN); // Init each MFRC522 card
    mfrc522[reader].PCD_DumpVersionToSerial();
  }
}

void Rfid1_loop() {
    
    if (mfrc522[0].PICC_IsNewCardPresent() && mfrc522[0].PICC_ReadCardSerial()) {
      dump_byte_array(mfrc522[0].uid.uidByte, mfrc522[0].uid.size);
      MFRC522::PICC_Type piccType = mfrc522[0].PICC_GetType(mfrc522[0].uid.sak);
      // Halt PICC
      mfrc522[0].PICC_HaltA();
      // Stop encryption on PCD
      mfrc522[0].PCD_StopCrypto1();
      if(mfrc522[0].uid.uidByte[0] == 211 && mfrc522[0].uid.uidByte[1] == 67 && mfrc522[0].uid.uidByte[2] == 22 && mfrc522[0].uid.uidByte[3] == 19) {    //태그 ID가 일치할경우 
         Rfid_Output1 = 1;
         Serial.println("rfid1 tag correct");
      }
      else if(mfrc522[0].uid.uidByte[0] == 99 && mfrc522[0].uid.uidByte[1] == 126 && mfrc522[0].uid.uidByte[2] == 122 && mfrc522[0].uid.uidByte[3] == 13) {    //태그 ID가 일치할경우 
         Rfid_Output1 = 1;
         Serial.println("rfid1 tag correct");
      }
      else {                                   // 다른 태그 ID일 경우
         Serial.println("rfid1 tag incorrect");
      } 
    }

   
}


void Rfid2_loop() {

    if (mfrc522[1].PICC_IsNewCardPresent() && mfrc522[1].PICC_ReadCardSerial()) {
      dump_byte_array(mfrc522[1].uid.uidByte, mfrc522[1].uid.size);
      MFRC522::PICC_Type piccType = mfrc522[1].PICC_GetType(mfrc522[1].uid.sak);
      // Halt PICC
      mfrc522[1].PICC_HaltA();
      // Stop encryption on PCD
      mfrc522[1].PCD_StopCrypto1();
      if(mfrc522[1].uid.uidByte[0] == 163 && mfrc522[1].uid.uidByte[1] == 183 && mfrc522[1].uid.uidByte[2] == 229 && mfrc522[1].uid.uidByte[3] == 11) {    //태그 ID가 일치할경우 
         Rfid_Output2 = 1;
         Serial.println("rfid2 tag correct");
      }
      else if(mfrc522[1].uid.uidByte[0] == 35 && mfrc522[1].uid.uidByte[1] == 82 && mfrc522[1].uid.uidByte[2] == 249 && mfrc522[1].uid.uidByte[3] == 17) {    //태그 ID가 일치할경우 
         Rfid_Output2 = 1;
         Serial.println("rfid2 tag correct");
      }
      else {                                   // 다른 태그 ID일 경우
         Serial.println("rfid2 tag incorrect");
      } 
    } 
}

void Rfid3_loop() {
    if (mfrc522[2].PICC_IsNewCardPresent() && mfrc522[2].PICC_ReadCardSerial()) {
      dump_byte_array(mfrc522[2].uid.uidByte, mfrc522[2].uid.size);
      MFRC522::PICC_Type piccType = mfrc522[2].PICC_GetType(mfrc522[2].uid.sak);
      // Halt PICC
      mfrc522[2].PICC_HaltA();
      // Stop encryption on PCD
      mfrc522[2].PCD_StopCrypto1();
      if(mfrc522[2].uid.uidByte[0] == 147 && mfrc522[2].uid.uidByte[1] == 60 && mfrc522[2].uid.uidByte[2] == 12 && mfrc522[2].uid.uidByte[3] == 12) {    //태그 ID가 일치할경우 
         Rfid_Output3 = 1;
         Serial.println("rfid3 tag correct");
      }
      else if(mfrc522[2].uid.uidByte[0] == 35 && mfrc522[2].uid.uidByte[1] == 11 && mfrc522[2].uid.uidByte[2] == 230 && mfrc522[2].uid.uidByte[3] == 17) {    //태그 ID가 일치할경우 
         Rfid_Output3 = 1;
         Serial.println("rfid3 tag correct");
      }
      else {                                   // 다른 태그 ID일 경우
         Serial.println("rfid3 tag incorrect");
      } 
    } 
}

  void dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}
