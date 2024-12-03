#include "Particle.h"

// Variables and objects for the first code segment
int servoPin = D1;
Servo serv;
SerialLogHandler logHandler(LOG_LEVEL_INFO);

// Variables for the second code segment
const int tiltPin = D2;
unsigned long lastPublish = 0;
const unsigned long PUBLISH_INTERVAL = 2000; // Interval for publishing results
const unsigned long SHAKE_WINDOW = 2000;     // Time window for detecting shakes (2 seconds)
const int SHAKE_THRESHOLD = 10;              // Minimum number of state changes to detect shaking
int lastState = 0;
int stateChanges = 0;
unsigned long windowStart = 0;

SYSTEM_MODE(AUTOMATIC);

void setup() {
    // Initialization for the first code segment
    serv.attach(servoPin);
    Particle.function("shake", shake);

    // Initialization for the second code segment
    Serial.begin(115200);
    pinMode(tiltPin, INPUT_PULLUP);
    windowStart = millis();
}

int shake(String cmd) {
    // Function to control the servo based on command
    if (cmd == "1") {
        serv.write(30);
        delay(1000);
        serv.write(-30);
        delay(1000);
    }
    delay(1000);
    int deg = cmd.toInt();
    serv.write(deg);
    return 1;
}

void loop() {
    // Loop logic from the second code segment
    int currentState = digitalRead(tiltPin);
    unsigned long currentTime = millis();

    // Detect state changes
    if (currentState != lastState) {
        stateChanges++;
        lastState = currentState;
        Serial.print("State change: ");
        Serial.println(stateChanges);
    }

    // Check if the end of the time window is reached
    if (currentTime - windowStart >= SHAKE_WINDOW) {
        bool isShaking = (stateChanges >= SHAKE_THRESHOLD);
        
        // Publish results to the cloud
        if (currentTime - lastPublish >= PUBLISH_INTERVAL) {
            String data = String(isShaking ? "1" : "0");
            Particle.publish("shake-detect", data, PRIVATE);
            Serial.print("Shake detected: ");
            Serial.println(isShaking ? "YES" : "NO");
            lastPublish = currentTime;
        }
        
        // Reset the counter and time window
        stateChanges = 0;
        windowStart = currentTime;
    }

    delay(10);  // Short delay to increase the sampling rate
}
