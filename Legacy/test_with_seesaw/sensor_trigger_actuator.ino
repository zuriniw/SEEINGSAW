//这段代码能实现按按钮，linear actuator回缩，但是当sensor detect到物体时，只有led亮，lineat actuator不工作

bool doBlink = false;
int ledPin = D2;
int sensorPin = D3;
int buttonPin = D4;  // Button pin for manual retraction
bool isRetracting = false;  // Track retraction state

// L298N control pins
int enPin = A2;    // Enable pin for speed control
int in1 = D6;      // Direction control 1
int in2 = D7;      // Direction control 2

bool isBlinking = false;
int blinkCount = 0;
unsigned long lastBlinkTime = 0;

void setup() {
    pinMode(ledPin, OUTPUT);
    pinMode(enPin, OUTPUT);
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(buttonPin, INPUT_PULLUP);
    
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(enPin, HIGH);
    
    blinkLED(3, ledPin);
    Particle.subscribe("blinkLED", handleActivateLED);
}

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
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
}

void actuatorRetract() {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
}

void actuatorStop() {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    analogWrite(enPin, 0);
}


void loop() {
    unsigned long currentMillis = millis();
    
    // Handle sensor and actuator control first
    if (!isRetracting) {
        unsigned long sensorValue = 0;
        for(int i = 0; i < 5; i++) {
            sensorValue += readQTR();
            delay(1);
        }
        sensorValue /= 5;

        if(sensorValue < 3000) {
            actuatorExtend();
            if (!isBlinking) digitalWrite(ledPin, HIGH);
            Particle.publish("doPairedPublish");
            // Remove delay here - it's interrupting actuator operation
        } else {
            if (!isBlinking) digitalWrite(ledPin, LOW);
            actuatorStop();
        }
    }

    // Handle button control
    if (digitalRead(buttonPin) == LOW) {
        isRetracting = true;
        actuatorRetract();
        if (!isBlinking) digitalWrite(ledPin, HIGH);
    } else if (isRetracting) {
        isRetracting = false;
        actuatorStop();
        if (!isBlinking) digitalWrite(ledPin, LOW);
    }
    
    // Handle LED blinking last
    if (doBlink && !isBlinking) {
        isBlinking = true;
        blinkCount = 0;
        lastBlinkTime = currentMillis;
    }
    
    if (isBlinking) {
        if (currentMillis - lastBlinkTime >= 500) {
            if (blinkCount >= 12) {
                isBlinking = false;
                doBlink = false;
                digitalWrite(ledPin, LOW);
            } else {
                digitalWrite(ledPin, !digitalRead(ledPin));
                lastBlinkTime = currentMillis;
                blinkCount++;
            }
        }
    }
    
    delay(10);
}


void blinkLED(int times, int pin) {
    for(int i = 0; i < times; i++) {
        digitalWrite(pin, HIGH);
        delay(500);
        digitalWrite(pin, LOW);
        delay(500);
    }
}

void handleActivateLED(const char *event, const char *data) {
    doBlink = true;
}
