bool doBlink = false;
int ledPin = D2;
int sensorPin = D3;

void setup() {
    pinMode(ledPin, OUTPUT);
    blinkLED(3, ledPin);
    Particle.subscribe("blinkLED", handleActivateLED);
}

// Function to read QTR-1RC sensor
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

void loop() {
    // Read and average sensor values
    unsigned long sensorValue = 0;
    for(int i = 0; i < 5; i++) {
        sensorValue += readQTR();
        delay(1);
    }
    sensorValue /= 5;

    if(sensorValue < 1000) {
        digitalWrite(ledPin, HIGH);
        Particle.publish("doPairedPublish");
    } else {
        digitalWrite(ledPin, LOW);
    }
    
    delay(1000);

    if(doBlink == true) {
        blinkLED(6, ledPin);
        doBlink = false;
    }
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
