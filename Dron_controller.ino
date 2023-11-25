//이륜드론 컨트롤러 코드

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8);
const byte address[6] = "02019"; //송신, 수신부분의 주소가 일치하여야 통신가능 즉 양쪽다 같은 숫자로 맞출것
int x_key = A0;       //컨트롤러모듈 핀번호 배치
int y_key = A1;
int z_key = 6;
int r2x_key = A2;
int r2y_key = A3;
int r2z_key = 5;
struct Data{ //컨트롤러 신호를 구조체로 묶어서 송신
  byte joyX;
  byte joyY;
  byte joyZ;
  byte r2joyX;
  byte r2joyY;
  byte r2joyZ;
};
Data data;

void setup() {
  Serial.begin(9600); //nRF24L01모듈 테스트전 시리얼로 확인 필요
  radio.begin();
  radio.setAutoAck(true);
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX); //라디오 통신값 최대로설정 (MIN, LOW, HIGH, MAX 4가지 값이있음)
  radio.stopListening();
  pinMode(z_key,INPUT_PULLUP); //z축의 경우 송수신을 위해 PULLUP으로 바꿔 0, 1로 송신하도록 설정
  pinMode(r2z_key,INPUT_PULLUP);
}

void loop() {
  data.joyX = map(analogRead(x_key),0,1023,0,179); //map함수로 컨트롤러의 범위 간소화
  data.joyY = map(analogRead(y_key),0,1023,0,179);
  data.joyZ = digitalRead(z_key);
  data.r2joyX = map(analogRead(r2x_key),0,1023,0,179);
  data.r2joyY = map(analogRead(r2y_key),0,1023,0,179);
  data.r2joyZ = digitalRead(r2z_key);
  
  Serial.println(data.joyX); //송수신 전 입력테스트
  Serial.println(data.joyY);
  Serial.println(data.r2joyX);
  Serial.println(data.r2joyY);
  Serial.println(data.joyZ);
  Serial.println(data.r2joyZ);
  
  radio.write(&data, sizeof(Data)); //송신
  delay(10);
}
