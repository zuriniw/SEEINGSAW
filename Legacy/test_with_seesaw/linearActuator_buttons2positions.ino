#include "Particle.h"

// Button pin definitions
int button_mid = S3;            
int button_re = SCK;             
int button_ex = MOSI;             
int ledpin = D2;

// L298N Motor Driver control pins
int enPin = A2;    
int in1 = D6;      
int in2 = D7;      

// Actuator properties
const int totalLength = 50;  // mm
const int speed = 40;  // mm/s
const int midPositionTimeMs = (totalLength * 1000) / (2 * speed); 

// Position tracking
int currentPosition = 0;  // 0 = fully retracted, 50 = fully extended
bool isMoving = false;

void setup() {
    pinMode(enPin, OUTPUT);
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(button_mid, INPUT_PULLUP);
    pinMode(button_re, INPUT_PULLUP);
    pinMode(button_ex, INPUT_PULLUP);
    
    blinkLED(2, ledpin);

    // Initialize to retracted position
    actuatorRetract();
    delay(2000);
    currentPosition = 0;  // Set initial position

    // Move to mid position
    moveToPosition(totalLength/2);  // Move to 25mm
}

void loop() {
    if (digitalRead(button_mid) == LOW && !isMoving) {
        moveToPosition(totalLength/2);  // Move to 25mm
    } 
    else if (digitalRead(button_re) == LOW && !isMoving) {
        moveToPosition(0);  // Fully retract
    }
    else if (digitalRead(button_ex) == LOW && !isMoving) {
        moveToPosition(totalLength);  // Fully extend
    }
}

void moveToPosition(int targetPosition) {
    if (targetPosition == currentPosition) return;
    
    isMoving = true;
    int distanceToMove = abs(targetPosition - currentPosition);
    int moveTime = (distanceToMove * 1000) / speed;
    
    if (targetPosition > currentPosition) {
        actuatorExtend();
    } else {
        actuatorRetract();
    }
    
    delay(moveTime);
    actuatorStop();
    
    currentPosition = targetPosition;
    isMoving = false;
}


void actuatorExtend() {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(enPin, 200);  // Use PWM for speed control
}

void actuatorRetract() {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    analogWrite(enPin, 200);  // Use PWM for speed control
}

void actuatorStop() {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    analogWrite(enPin, 0);  // Stop the motor
}

void blinkLED(int times, int pin) {
    for(int i = 0; i < times; i++) {
        digitalWrite(pin, HIGH);
        delay(100);
        digitalWrite(pin, LOW);
        delay(100);
    }
}
