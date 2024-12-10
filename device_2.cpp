///////////////////////////////////////////////////////////////////////////////
//  PAIRED DEVICE --> leslie's end (device 2)
///////////////////////////////////////////////////////////////////////////////
bool isBlueOn = false;
bool isObjectIn_1 = false;
bool isObjectIn_2 = false;
bool last_isObjectIn_2 = false;

// Button pin definitions
int button_mid = S3;            
int button_re = SCK;             
int button_ex = MOSI;   

int ledPin = D2;                // led(RED) is the indicator of both-sides state, aligned with actuator position state
int yellowPin = D0;             // YELLOW LED is the indicator of self-sides state
int bluePin = D1;                // BLUE LED is the indicator of other-sides state

int sensorPin = D3;

// L298N control pins
int enPin = A2;    // Enable pin for speed control
int in1 = D6;      // Direction control 1
int in2 = D7;      // Direction control 2

// Actuator properties
const int totalLength = 50;  // mm
const int speed = 40;  // mm/s
const int midPositionTimeMs = (totalLength * 1000) / (2 * speed); 

// Position tracking
int currentPosition = 0;  // 0 = fully retracted, 50 = fully extended
bool isMoving = false;

////////////////////////////////////////////////////////////////////////////////
void setup() {
    pinMode(ledPin, OUTPUT);
    pinMode(enPin, OUTPUT);
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    
    pinMode(yellowPin, OUTPUT);
    pinMode(bluePin, OUTPUT);

    pinMode(button_mid, INPUT_PULLUP);
    pinMode(button_re, INPUT_PULLUP);
    pinMode(button_ex, INPUT_PULLUP);
    
    // Initialize actuator to stopped state
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(enPin, HIGH);
    
    digitalWrite(bluePin, LOW);
    
    blinkLED(3, ledPin);
    Particle.subscribe("doBlue_1", handleLED);

    // Initialize to retracted position
    actuatorRetract();
    delay(2000);
    currentPosition = 0;  // Set initial position

    // Move to mid position
    moveToPosition(totalLength/2);  // Move to 25mm
}

////////////////////////////////////////////////////////////////////////////////
void loop() {
    
    unsigned long sensorValue = 0;
    for(int i = 0; i < 5; i++) {
        sensorValue += readQTR();
        delay(1);
    }
    sensorValue /= 5;

    if(sensorValue < 1000) {
        // object presence in 2
        digitalWrite(yellowPin, HIGH);
        isObjectIn_2 = true;             // set the state of 2 itself, according to the sensor
  
    } else {
        // no object in 2
        digitalWrite(yellowPin, LOW);
        isObjectIn_2 = false;             // set the state of 2 itself, according to the sensor
    }
    
    unsigned long lastPublishTime = 0;  // 记录上次发布时间
    const unsigned long PUBLISH_INTERVAL = 1000;  // 发布间隔为1秒
    
    if (isObjectIn_1 != last_isObjectIn_1) {
        unsigned long currentTime = millis();
        
        if (currentTime - lastPublishTime >= PUBLISH_INTERVAL) {
            if (isObjectIn_2 == true) {   
                Particle.publish("doBlue_2", "1");
            } else {
                Particle.publish("doBlue_2", "0");
            }
            lastPublishTime = currentTime;  // 更新上次发布时间
            last_isObjectIn_2 = isObjectIn_2;
        }
    }


    // go to mid 
    if (isObjectIn_1 == isObjectIn_2) {
        blinkLED(1, ledPin);
        moveToPosition(totalLength/2);
    }
    
    // retract --> the end sink
    if (isObjectIn_1 == false and isObjectIn_2 == true) {
        blinkLED(3, ledPin);
        moveToPosition(totalLength); 
    }

    // extend --> the end lift
    if (isObjectIn_1 == true and isObjectIn_2 == false) {
        blinkLED(6, ledPin);
        moveToPosition(0);
    }
    
    ////  button control  ////
    
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

////////////////////////////////////////////////////////////////////////////////
void setBlueOn() {
    digitalWrite(bluePin, HIGH);
}

void setBlueOff() {
    digitalWrite(bluePin, LOW);
}

void handleLED(const char *event, const char *data) {
    if (*data == '1'){
        setBlueOn();
        isObjectIn_1 = true;             // set the state of 1, according to the data broadcase
    }
    if (*data == '0'){
        setBlueOff();
        isObjectIn_1 = false;             // set the state of 1, according to the data broadcase
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
unsigned long readQTR() {
    unsigned long duration = 0;
    pinMode(sensorPin, OUTPUT);
    digitalWrite(sensorPin, HIGH);
    delayMicroseconds(10);
    
    pinMode(sensorPin, INPUT);
    while(digitalRead(sensorPin) == HIGH && duration < 3000) {
        duration++;
    }
    return duration;
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
    // Optionally control speed with PWM
    analogWrite(enPin, 200);  // PWM value for speed control
}

void actuatorRetract() {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    // Optionally control speed with PWM
    analogWrite(enPin, 200);  // PWM value for speed control
}

void actuatorStop() {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    analogWrite(enPin, 0);
}


void blinkLED(int times, int pin) {
    for(int i = 0; i < times; i++) {
        digitalWrite(pin, HIGH);
        delay(100);
        digitalWrite(pin, LOW);
        delay(100);
    }
}
////////////////////////////////////////////////////////////////////////////////
