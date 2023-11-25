//드론 본체 코드

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>
#define SPMAX 75 //모터 최대출력값 지정 BLDC모터이기 때문에 75도 상당히 위험함 따라서 낮춰쓰는것을 추천
Servo motor1,motor2;
float val1 = 60; //왼쪽바퀴 기본 모터값 설정
float val2 = 60; //오른쪽바퀴 기본 모터값 설정
int LED = 9; //LED는 5V를 사용해서 추가 외부전원 필요없음
RF24 radio(7, 8);
const byte address[6] = "02019"; //송신기와 동일하게 설정 컨트롤러에도 02019로 설정되 있을 것임
struct Data{
  byte joyX;
  byte joyY;
  byte joyZ;
  byte r2joyX;
  byte r2joyY;
  byte r2joyZ;
  };
Data data;

void setup() {
  Serial.begin(9600);
  pinMode(LED,OUTPUT);
  radio.begin();
  radio.setAutoAck(true);
  motor1.attach (5);
  motor2.attach (6);
  motor1.write(60);
  motor2.write(60);
  delay(3000);
  
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX); //수신과 동일하게 MAX로 설정(MIN, LOW, HIGH, MAX)
  radio.startListening(); //데이터 수신
}

void loop() {
  while (radio.available()) {
    radio.read(&data, sizeof(Data));
    if (val1 > SPMAX || val2 > SPMAX){ //속도가 지정한 최대값이 넘으면 다시 60으로 고정
      val1 = 60;
      val2 = 60;
    }
    else{
      if(data.joyY < 10){ //가속
        val1 += 0.1;
        val2 += 0.1;
      }
      if(data.joyY > 169){ // 감속
        val1 -= 0.1;
        val2 -= 0.1;
      }
      if(data.joyX > 169){ //오른쪽 속도
        val2 += 0.1;
      }
      if(data.joyX < 10){ //왼쪽 속도
        val1 += 0.1;
      }
      if(data.r2joyX>169){ //비상정지
        val1 = 60;
        val2 = 60;
      }
      if(data.r2joyX<10){ //낮은쪽에 속도에 맞추기
        if (val1>val2){
          val1 = val2;}
        else{
          val2 = val1;
        }
      }
      if (data.r2joyY>169){ //출발()
        val1 = 65.8;
        val2 = 65.8;
      }
      if (data.r2joyY<10){ //시동(휠에 따라 다르지만 대략 63.5부터 모터가 움직이기 시작함)
        val1 = 65;
        val2 = 65;
     }
      motor1.write(val1);
      motor2.write(val2);
    }
    if(data.joyZ == 0){ //LED켜기
      digitalWrite(LED, HIGH);
    }
    if(data.r2joyZ == 0){ //LED끄기
      digitalWrite(LED, LOW);
    }
  }
    Serial.println(data.joyX);
    Serial.println(data.joyY);
    Serial.print("value 1 :");
    Serial.println(val1);
    Serial.print("value 2 :");
    Serial.println(val2);
    delay(10); 

}
