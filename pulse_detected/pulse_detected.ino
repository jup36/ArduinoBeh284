// Constants
const int inputPin = 2;    // Digital input pin for pulse detection
const int outputPin = 6;   // Digital output pin for pulse generation

// Variables
volatile unsigned long pulseStartTime = 0; // Holds the time of the incoming pulse
volatile bool pulseDetected = false;       // Flag to indicate if a pulse is detected

// Pulse parameters
const unsigned long pulseDuration = 100;   // Duration of the output pulse (in milliseconds)

void setup() {
  pinMode(inputPin, INPUT);
  pinMode(outputPin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(inputPin), pulseDetection, CHANGE); // Attach the interrupt to the rising edge of the input pulse
}

void loop() {
  // Check if a pulse is detected
  if (pulseDetected) {
    pulseDetected = false; // Reset the flag
    // Generate the output pulse
    digitalWrite(outputPin, HIGH);
    delay(pulseDuration);
    digitalWrite(outputPin, LOW);
  }
  // Add any other non-blocking tasks in your loop if needed
}

// Interrupt service routine for pulse detection
void pulseDetection() {
  pulseStartTime = millis(); // Record the start time of the incoming pulse
  pulseDetected = true;      // Set the flag to indicate a pulse is detected
}
