// define input pins 
const int switchPin = 4; // manual switch
const int lickPin = 8;  // lickometer
// define output pins
const int waterPin = 2; // water
const int airPin = 3;   // air

// Variables
//bool switchDetected = false; // switch flag
unsigned long lastLickStateChange = 0; // Last time lickometer gone up
unsigned long lastSwitchStateChange = 0; // Last time switch gone up

// Pulse parameters
const unsigned long waterDur = 20;   // Duration of the water pulse (ms)
const unsigned long airDur = 20;   // Duration of the air pulse (ms)


void setup() {
  pinMode(lickPin, INPUT);
  pinMode(switchPin, INPUT);
  pinMode(waterPin, OUTPUT);
  pinMode(airPin, OUTPUT);
}

void loop() {
  // Poll the input pin to detect the pulse
  int lickState = digitalRead(lickPin);
  int switchState = digitalRead(switchPin); 

  if (serialData == ){  
    // Check if the lickometer state has changed
    if (lickState == HIGH && lastLickStateChange == 0) {
      lastLickStateChange = millis(); // Record the time of the lickometer rising edge
    } else if (lickState == LOW && lastLickStateChange > 0) {
      // Calculate the lick pulse width
      unsigned long lickPulseWidth = millis() - lastLickStateChange;
      // Check if the lick pulse width is within a valid range (adjust the range as needed)
      if (lickPulseWidth > 10 && lickPulseWidth < 500) {
        // Pulse detected, trigger the output pulse
        digitalWrite(airPin, HIGH);
        delay(airDur);
        digitalWrite(airPin, LOW);
      }
    // Reset the variables for the next pulse detection
    lastLickStateChange = 0;
  }
  }

  // Check if the switch state has changed
  if (switchState == HIGH && lastSwitchStateChange == 0) {
    lastSwitchStateChange = millis(); // Record the time of the lickometer rising edge
  } else if (switchState == LOW && lastSwitchStateChange > 0) {
    // Calculate the switch pulse width
    unsigned long switchPulseWidth = millis() - lastSwitchStateChange;
    // Check if the switch pulse width is within a valid range (adjust the range as needed)
    if (switchPulseWidth > 10 && switchPulseWidth < 500) {
      // Pulse detected, trigger the output pulse
      digitalWrite(airPin, HIGH);
      delay(airDur);
      digitalWrite(airPin, LOW);
    }
    // Reset the variables for the next pulse detection
    lastSwitchStateChange = 0;
  }
  
}
