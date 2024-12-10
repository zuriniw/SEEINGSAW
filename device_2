///////////////////////////////////////////////////////////////////////////////
//  PAIRED DEVICE --> leslie's end (device 2)
///////////////////////////////////////////////////////////////////////////////
bool isBlueOn = false;
bool isObjectIn_1 = false;
bool isObjectIn_2 = false;

int ledPin = D2;                // led(RED) is the indicator of both-sides state, aligned with actuator position state
int yellowPin = D0;             // YELLOW LED is the indicator of self-sides state
int bluePin = D1;                // BLUE LED is the indicator of other-sides state

int sensorPin = D3;

// L298N control pins
int enPin = A2;    // Enable pin for speed control
int in1 = D6;      // Direction control 1
int in2 = D7;      // Direction control 2

////////////////////////////////////////////////////////////////////////////////
void setup() {
    pinMode(ledPin, OUTPUT);
    pinMode(enPin, OUTPUT);
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    
    pinMode(yellowPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
    
    // Initialize actuator to stopped state
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(enPin, HIGH);
    
    digitalWrite(bluePin, LOW);
    
    blinkLED(3, ledPin);
    
    // recieve messages from device 1
    Particle.subscribe("doBlue_1", handleLED);
}

////////////////////////////////////////////////////////////////////////////////
void loop() {
    unsigned long sensorValue = 0;
    for(int i = 0; i < 5; i++) {
        sensorValue += readQTR();
        delay(1);
    }
    sensorValue /= 5;

    if(sensorValue < 3000) {
        digitalWrite(ledPin, HIGH);
        digitalWrite(yellowPin, HIGH);
        Particle.publish("doBlue_2", "1");
        isObjectIn_2 = true;             // set the state of 2 itself, according to the sensor
    } else {
        digitalWrite(ledPin, LOW);
        digitalWrite(yellowPin, LOW);
        Particle.publish("doBlue_2", "0");
        isObjectIn_2 = false;             // set the state of 2 itself, according to the sensor
    }
    
    delay(1000);
    
    // go to mid 
    if (isObjectIn_1 == isObjectIn_2) {
        blinkLED(1, ledPin);
    }
    
    // retract --> the end sink
    if (isObjectIn_1 == false and isObjectIn_2 == true) {
        blinkLED(3, ledPin);
    }

    // extend --> the end lift
    if (isObjectIn_1 == true and isObjectIn_2 == false) {
        blinkLED(6, ledPin);
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
