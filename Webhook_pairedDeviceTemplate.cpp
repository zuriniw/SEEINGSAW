// 移除INPUT_PULLUP，QTR-1RC需要使用特殊的timing方式读取
int sensorPin = D3;
int ledPin = D2;

void setup() {
    pinMode(ledPin, OUTPUT);
    Particle.subscribe("blinkLED", handleActivateLED);
    blinkLED(3, ledPin);
}

// 添加RC传感器读取函数
unsigned long readQTR() {
    unsigned long duration = 0;
    
    // 充电阶段
    pinMode(sensorPin, OUTPUT);
    digitalWrite(sensorPin, HIGH);
    delayMicroseconds(10);
    
    // 放电并计时阶段
    pinMode(sensorPin, INPUT);
    while(digitalRead(sensorPin) == HIGH) {
        duration++;
        if(duration > 3000) break; // 超时保护
    }
    
    return duration;
}

void loop() {
    // 读取传感器值
    unsigned long sensorValue = readQTR();
    
    // 设定一个阈值来判断是否检测到物体
    // 可能需要根据实际情况调整这个阈值
    if(sensorValue < 1000) {
        digitalWrite(ledPin, HIGH);
        Particle.publish("doPairedPublish");
    } else {
        digitalWrite(ledPin, LOW);
    }
    
    delay(100); // 降低采样频率
    
    if(doBlink) {
        blinkLED(6, ledPin);
        doBlink = false;
    }
}
