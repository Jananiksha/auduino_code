#include <Wire.h>
#include <Arduino.h>

// Motor A connections
const int enA = 9;
const int in1 = 8;
const int in2 = 7;
// Motor B connections
const int enB = 10;
const int in3 = 12;
const int in4 = 11;

// Encoder A connections
const int encoderAPinA = 2;
const int encoderAPinB = 3;
// Encoder B connections
const int encoderBPinA = 18;
const int encoderBPinB = 19;

volatile int encoderACount = 0;
volatile int encoderBCount = 0;

// Function prototypes
void encoderA_ISR();
void encoderB_ISR();
void motorControl(int motorSpeed, int motorDirection, int enPin, int in1Pin, int in2Pin);

void setup() {
    Serial.begin(9600);
  // Setup motor control pins
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  // Attach interrupts for encoders
  attachInterrupt(digitalPinToInterrupt(encoderAPinA), encoderA_ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderBPinA), encoderB_ISR, CHANGE);

    Wire.begin(4);
    Wire.onReceive(receiveEvent);
}

void loop() {
  // Sample motor control
  // motorControl(80, 1, enA, in1, in2); // Motor A at full speed clockwise
  // motorControl(80, 1, enB, in3, in4); // Motor B at 80% speed counterclockwise

  // Example: print encoder counts
  Serial.print("Encoder A count: ");
  Serial.println(encoderACount);
  Serial.print("Encoder B count: ");
  Serial.println(encoderBCount);
  delay(1000); // Adjust delay as needed
}


void receiveEvent(int numBytes) {
  while (Wire.available() > 0) {
    char command = Wire.read();
    if (command == 'F') {
      motorControl(80, 1, enA, in1, in2); 
      motorControl(80, 0, enB, in3, in4);
    }
    else if (command == 'B') {
      motorControl(80, 0, enA, in1, in2); 
      motorControl(80, 1, enB, in3, in4);
    }
  }
}


void encoderA_ISR() {
  if (digitalRead(encoderAPinB) == HIGH) {
    encoderACount++;
  } else {
    encoderACount--;
  }
}

void encoderB_ISR() {
  if (digitalRead(encoderBPinB) == HIGH) {
    encoderBCount++;
  } else {
    encoderBCount--;
  }
}

void motorControl(int motorSpeed, int motorDirection, int enPin, int in1Pin, int in2Pin) {
  // Control motor speed and direction
  digitalWrite(in1Pin, motorDirection);
  digitalWrite(in2Pin, !motorDirection);
  analogWrite(enPin, motorSpeed);
}