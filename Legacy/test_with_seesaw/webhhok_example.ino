void setup() {
  // Subscribe to the integration response event
  Particle.subscribe("hook-response/doPairedPublish", myHandler, MY_DEVICES);
}

void myHandler(const char *event, const char *data) {
  // Handle the integration response
}
void loop() {
  // Get some data
  String data = String(10);
  // Trigger the integration
  Particle.publish("doPairedPublish", data, PRIVATE);
  // Wait 60 seconds
  delay(60000);
}
