#include <Arduino.h>

const int enA = 9; // Motor A enable pin
const int in1 = 8; // Motor A input pin 1
const int in2 = 7; // Motor A input pin 2
// Motor B connections
const int enB = 10; // Motor B enable pin
const int in3 = 12; // Motor B input pin 1
const int in4 = 11; // Motor B input pin 2

// Encoder A connections
const int encoderAPinA = 2; // Encoder A pin A
const int encoderAPinB = 3; // Encoder A pin B
// Encoder B connections
const int encoderBPinA = 18; // Encoder B pin A
const int encoderBPinB = 19; // Encoder B pin B

volatile unsigned long encoderATime = 0; // Time of last encoder A pulse
volatile unsigned long encoderBTime = 0; // Time of last encoder B pulse
volatile int encoderACount = 0; // Encoder A count
volatile int encoderBCount = 0; // Encoder B count

void setup() {
  Serial.begin(9600);

  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  pinMode(encoderAPinA, INPUT_PULLUP);
  pinMode(encoderAPinB, INPUT_PULLUP);
  pinMode(encoderBPinA, INPUT_PULLUP);
  pinMode(encoderBPinB, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(encoderAPinA), incrementEncoderA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderAPinB), incrementEncoderA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderBPinA), incrementEncoderB, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderBPinB), incrementEncoderB, CHANGE);
}

void loop() {
  if (millis() % 1000 == 0) { // Log every second
    float encoderARPM = 0;
    float encoderBRPM = 0;
    if (encoderACount > 0) {
      encoderARPM = ((float)(encoderACount * 60) / (millis() - encoderATime));
    }
    if (encoderBCount > 0) {
      encoderBRPM = ((float)(encoderBCount * 60) / (millis() - encoderBTime));
    }

    Serial.print("Motor A Count: ");
    Serial.print(encoderACount);
    Serial.print(", RPM: ");
    Serial.println(encoderARPM);
    Serial.print("Motor B Count: ");
    Serial.print(encoderBCount);
    Serial.print(", RPM: ");
    Serial.println(encoderBRPM);

    encoderACount = 0;
    encoderBCount = 0;
    encoderATime = millis();
    encoderBTime = millis();
  }

  // Example: Make both motors rotate forward at maximum speed
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(enA, 255); // Full speed

  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enB, 255); // Full speed
}

void incrementEncoderA() {
  static bool lastStateA = digitalRead(encoderAPinA);
  bool stateA = digitalRead(encoderAPinA);
  if (lastStateA!= stateA) {
    encoderACount++;
    encoderATime = millis();
  }
  lastStateA = stateA;
}

void incrementEncoderB() {
  static bool lastStateB = digitalRead(encoderBPinA);
  bool stateB = digitalRead(encoderBPinB);
  if (lastStateB!= stateB) {
    encoderBCount++;
    encoderBTime = millis();
  }
  lastStateB = stateB;
}
