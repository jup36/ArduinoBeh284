const int pinToTest = 3; // Original pin
const int mirrorPin = 5; // Pin to mirror the behavior of pinToTest
const int pulseCount = 10; // Number of pulses to generate
const int pulseDuration = 40; // Duration (ms) of each half cycle in milliseconds (50% duty cycle for 1Hz)
const int pulseDelay = 1000; 

void setup() {
  pinMode(pinToTest, OUTPUT); // Set pinToTest as an output
  pinMode(mirrorPin, OUTPUT); // Set mirrorPin as an output
  digitalWrite(pinToTest, LOW); // Initialize pinToTest to LOW
  digitalWrite(mirrorPin, LOW); // Initialize mirrorPin to LOW
}

void setPins(int state) {
  digitalWrite(pinToTest, state);
  digitalWrite(mirrorPin, state);
}

void loop() {
  for (int i = 0; i < pulseCount; i++) {
    setPins(HIGH); // Set both pins HIGH
    delay(pulseDuration); // Wait for the duration of the HIGH pulse
    setPins(LOW); // Set both pins LOW
    delay(pulseDelay); // Wait for the duration of the LOW pulse
  }

  // Halt the loop after generating the pulses
  while (true);
}
