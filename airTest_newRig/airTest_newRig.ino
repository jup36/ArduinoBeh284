// Define the pin for the air output
const int airPin = 3;

// Define the pulse duration in milliseconds
const int airDur = 1000; // Adjust the duration as needed

void setup() {
  // Set the air pin as an output
  pinMode(airPin, OUTPUT);

  // Initialize serial communication at 9600 bits per second
  Serial.begin(9600);
}

void loop() {
  // Check if data is available to read from the serial port
  if (Serial.available() > 0) {
    // Read the incoming byte
    char incomingByte = Serial.read();

    // Check if the incoming byte is 'P'
    if (incomingByte == 'P') {
      // Generate a 5V pulse through the air pin
      digitalWrite(airPin, HIGH); // Set the air pin high
      delay(airDur);              // Wait for the specified duration
      digitalWrite(airPin, LOW);  // Set the air pin low
    }
  }
}
