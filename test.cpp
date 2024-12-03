#include "Particle.h"

// Variables for the servo control
int servoPin = D1;
Servo serv;
SerialLogHandler logHandler(LOG_LEVEL_INFO);

// Variables for the tilt sensor
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
    // Initialization for the servo
    serv.attach(servoPin);

    // Initialization for the tilt sensor
    Serial.begin(115200);
    pinMode(tiltPin, INPUT_PULLUP);
    windowStart = millis();

    // Subscribe to the event published by the tilt sensor
    Particle.subscribe("shake-detect", handleShake, MY_DEVICES);
}

void handleShake(const char *event, const char *data) {
    // Act based on the shake detection
    if (String(data) == "1") {
        Serial.println("Shake detected - Activating servo!");
        serv.write(30);
        delay(1000);
        serv.write(-30);
        delay(1000);
    }
}

void loop() {
    // Reading tilt sensor state and managing the detection logic
    int currentState = digitalRead(tiltPin);
    unsigned long currentTime = millis();

    if (currentState != lastState) {
        stateChanges++;
        lastState = currentState;
    }

    if (currentTime - windowStart >= SHAKE_WINDOW) {
        if (stateChanges >= SHAKE_THRESHOLD) {
            // Check if it's time to publish
            if (currentTime - lastPublish >= PUBLISH_INTERVAL) {
                Particle.publish("shake-detect", "1", PRIVATE);
                lastPublish = currentTime;
            }
        } else {
            // Reset if no shake detected within the window
            if (currentTime - lastPublish >= PUBLISH_INTERVAL) {
                Particle.publish("shake-detect", "0", PRIVATE);
                lastPublish = currentTime;
            }
        }

        // Reset for the next detection window
        stateChanges = 0;
        windowStart = currentTime;
    }

    delay(10);  // Short delay to increase the sampling rate
}
