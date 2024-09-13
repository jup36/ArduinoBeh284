const int pinToTest = 2; 
const int pulseCount = 10; // number of pulses to generate
const int pulseDuration = 500; // duration of each half cycle in milliseconds (50% duty cycle for 1Hz)

void setup() {
  pinMode(pinToTest, OUTPUT); // Set the water pin as an output
  digitalWrite(pinToTest, LOW); // Initialize the pin to LOW
}

void loop() {
  for (int i = 0; i < pulseCount; i++) {
    digitalWrite(pinToTest, HIGH); // Set the pin HIGH
    delay(pulseDuration); // Wait for the duration of the HIGH pulse
    digitalWrite(pinToTest, LOW); // Set the pin LOW
    delay(pulseDuration); // Wait for the duration of the LOW pulse
  }

  // Halt the loop after generating the pulses
  while(true);
}
