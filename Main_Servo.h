#include <Servo.h>
#include <SoftwareSerial.h>

// 핀 설정
const int trigPin = 5;        // 초음파 센서 Trig 핀
const int echoPin = 4;        // 초음파 센서 Echo 핀
const int speakerPin = 8;     // 부저 핀

// 블루투스 설정
SoftwareSerial PhoneBT(2, 3); // TX = 2, RX = 3

// 서보 모터 객체 생성
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

// 서보 모터 제어 핀 설정
const int servoPin1 = 9;
const int servoPin2 = 10;
const int servoPin3 = 11;
const int servoPin4 = 12;

// 서보 모터 중립 위치
const int neutralPosition = 90;

// 서보 모터 속도 설정 (0~180)
const int motorSpeed = 30; // 예시로 30도 회전

void setup() {
  PhoneBT.begin(9600);    // 블루투스 통신 시작
  Serial.begin(9600);     // 시리얼 통신 시작

  // 서보 모터 핀에 연결
  servo1.attach(servoPin1);
  servo2.attach(servoPin2);
  servo3.attach(servoPin3);
  servo4.attach(servoPin4);

  // 서보 모터를 중립 위치로 설정
  servo1.write(neutralPosition);
  servo2.write(neutralPosition);
  servo3.write(neutralPosition);
  servo4.write(neutralPosition);

  pinMode(trigPin, OUTPUT);   // 초음파 센서 Trig 핀 출력 설정
  pinMode(echoPin, INPUT);    // 초음파 센서 Echo 핀 입력 설정
  pinMode(speakerPin, OUTPUT); // 부저 핀 출력 설정
}

void loop() {
  // 블루투스 명령 수신 처리
  if (PhoneBT.available()) {
    char command = PhoneBT.read();
    Serial.print("수신된 명령: ");
    Serial.println(command);

    switch (command) {
      case 'F':
        Serial.println("전진");
        Go_Forward();
        break;
      case 'B':
        Serial.println("후진");
        Go_Backward();
        break;
      case 'L':
        Serial.println("좌회전");
        Go_Left();
        break;
      case 'R':
        Serial.println("우회전");
        Go_Right();
        break;
      case 'S':
        Serial.println("정지");
        Stop_Motors();
        break;
      default:
        Serial.println("알 수 없는 명령");
        break;
    }
  }

  // 초음파 센서를 이용한 거리 측정
  float distance = measureDistance();

  // 거리 기반 경고 및 긴급 정지 처리
  if (distance < 7) {
    Serial.println("긴급 정지");
    Emergency_Stop();
  } else if (distance < 13) {
    Serial.println("경고: 장애물 근접");
    Warning_Sound();
  }
}

// 거리 측정 함수
float measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  float distance = duration * 0.034 / 2; // cm 단위로 변환
  return distance;
}

// 서보 모터를 중립 위치로 설정하여 정지
void Stop_Motors() {
  servo1.write(neutralPosition);
  servo2.write(neutralPosition);
  servo3.write(neutralPosition);
  servo4.write(neutralPosition);
}

// 전진: 모든 서보 모터를 전진 방향으로 회전
void Go_Forward() {
  servo1.write(neutralPosition + motorSpeed);
  servo2.write(neutralPosition + motorSpeed);
  servo3.write(neutralPosition + motorSpeed);
  servo4.write(neutralPosition + motorSpeed);
}

// 후진: 모든 서보 모터를 후진 방향으로 회전
void Go_Backward() {
  servo1.write(neutralPosition - motorSpeed);
  servo2.write(neutralPosition - motorSpeed);
  servo3.write(neutralPosition - motorSpeed);
  servo4.write(neutralPosition - motorSpeed);
}

// 좌회전: 좌측 모터 후진, 우측 모터 전진
void Go_Left() {
  servo1.write(neutralPosition - motorSpeed);
  servo2.write(neutralPosition - motorSpeed);
  servo3.write(neutralPosition + motorSpeed);
  servo4.write(neutralPosition + motorSpeed);
}

// 우회전: 좌측 모터 전진, 우측 모터 후진
void Go_Right() {
  servo1.write(neutralPosition + motorSpeed);
  servo2.write(neutralPosition + motorSpeed);
  servo3.write(neutralPosition - motorSpeed);
  servo4.write(neutralPosition - motorSpeed);
}

// 경고음 발생
void Warning_Sound() {
  digitalWrite(speakerPin, HIGH);
  delay(500);
  digitalWrite(speakerPin, LOW);
  delay(500);
}

// 긴급 정지: 모터 정지 및 경고음 발생
void Emergency_Stop() {
  Stop_Motors();
  digitalWrite(speakerPin, HIGH);
  delay(1000);
  digitalWrite(speakerPin, LOW);
}
