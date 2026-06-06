#include <Servo.h>

// L298N
#define ENA 5
#define ENB 6

#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11

// Sensors
#define BACK_IR 2
#define FRONT_IR 3

// Servo
#define SERVO_PIN 4

// Buzzer
#define BUZZER 7

// Ultrasonic
#define TRIG 12
#define ECHO 13

Servo myservo;

const int SPEED = 150;
const int OBSTACLE_DISTANCE = 15;

void setup() {

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(FRONT_IR, INPUT);
  pinMode(BACK_IR, INPUT);

  pinMode(BUZZER, OUTPUT);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  myservo.attach(SERVO_PIN);
  myservo.write(90);

  Serial.begin(9600);
}

void loop() {

  int distance = getDistance();

  Serial.print("Distance: ");
  Serial.println(distance);

  // Front IR detect
  // Agar tumhara IR LOW par detect karta hai,
  // to HIGH ko LOW kar dena.
  if (digitalRead(FRONT_IR) == HIGH) {

    avoidObstacle();
    return;
  }

  // Ultrasonic detect
  if (distance <= OBSTACLE_DISTANCE) {

    avoidObstacle();
    return;
  }

  forward();
}

// =====================
// OBSTACLE AVOIDANCE
// =====================

void avoidObstacle() {

  beep();

  stopRobot();
  delay(100);

  // Reverse
  backward();
  delay(800);

  stopRobot();
  delay(100);

  // Servo Left
  myservo.write(150);
  delay(500);
  int leftDistance = getDistance();

  // Servo Right
  myservo.write(30);
  delay(500);
  int rightDistance = getDistance();

  // Center
  myservo.write(90);
  delay(300);

  if (leftDistance > rightDistance) {

    turnLeft();
    delay(600);

  } else {

    turnRight();
    delay(600);
  }

  stopRobot();
  delay(100);
}

// =====================
// BUZZER
// =====================

void beep() {

  digitalWrite(BUZZER, HIGH);
  delay(200);
  digitalWrite(BUZZER, LOW);
}

// =====================
// ULTRASONIC
// =====================

int getDistance() {

  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH, 25000);

  if (duration == 0)
    return 250;

  return duration * 0.034 / 2;
}

// =====================
// MOTOR FUNCTIONS
// =====================

void forward() {

  analogWrite(ENA, SPEED);
  analogWrite(ENB, SPEED);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void backward() {

  // Back sensor safety
  if (digitalRead(BACK_IR) == HIGH) {

    stopRobot();
    return;
  }

  analogWrite(ENA, SPEED);
  analogWrite(ENB, SPEED);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnLeft() {

  analogWrite(ENA, SPEED);
  analogWrite(ENB, SPEED);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnRight() {

  analogWrite(ENA, SPEED);
  analogWrite(ENB, SPEED);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopRobot() {

  analogWrite(ENA, 0);
  analogWrite(ENB, 0);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
