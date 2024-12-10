///////////////////////////////////////////////////////////////////////////////
//  PAIRED DEVICE --> ziru's end (device 1)
///////////////////////////////////////////////////////////////////////////////
bool isBlueOn = false;
bool isObjectIn_1 = false;
bool isObjectIn_2 = false;

int ledPin = D2;                // led(RED) is the indicator of both-sides state, aligned with actuator position state
int yellowPin = D0;              // YELLOW LED is the indicator of self-sides state
int bluePin = D1;                // BLUE LED is the indicator of other-sides state

int sensorPin = D3;

// L298N control pins
int enPin = A2;    // Enable pin for speed control
int in1 = D6;      // Direction control 1
int in2 = D7;      // Direction control 2

///////////////////////////////////////////////////////////////////////////////
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
    Particle.subscribe("doBlue_2", handleLED);
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
        // object presence in 1
        digitalWrite(yellowPin, HIGH);
        
        Particle.publish("doBlue_1", "1");// broadcast the state of 1
        isObjectIn_1 = true;             // set the state of 1 itself, according to the sensor
        
    } else {
        // no object in 1
        digitalWrite(yellowPin, LOW);
        
        Particle.publish("doBlue_1", "0");// broadcast the state of 1
        isObjectIn_1 = false;             // set the state of 1 itself, according to the sensor
        
    }
    //Serial.println("111");
    
    delay(1000);
    
    // go to mid 
    if (isObjectIn_1 == isObjectIn_2) {
        blinkLED(1, ledPin);
    }
    
    // retract --> the end sink
    if (isObjectIn_1 == true and isObjectIn_2 == false) {
        blinkLED(3, ledPin);
    }

    // extend --> the end lift
    if (isObjectIn_1 == false and isObjectIn_2 == true) {
        blinkLED(6, ledPin);
    }
}
////////////////////////////////////////////////////////////////////////////////
void setBlueOn() {
    digitalWrite(bluePin, HIGH);
    Serial.println("high");
}

void setBlueOff() {
    digitalWrite(bluePin, LOW);
    Serial.println("low");
}

void handleLED(const char *event, const char *data) {
    Serial.println(*data);
    if ( *data=='1'){
        setBlueOn();
        //Serial.println("got 0");
        isObjectIn_2 = true;
    }
    if (*data=='0'){
        setBlueOff();
        //Serial.println("got 1");
        isObjectIn_2 = false;
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
