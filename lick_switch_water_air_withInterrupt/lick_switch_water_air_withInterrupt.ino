int trialStep = 0;
// define input pins 
const int switchPin = 3; // manual switch
const int lickPin = 2;  // lickometer
// define output pins
const int waterPin = 5; // water
const int airPin = 6;   // air

// Variables
//bool switchDetected = false; // switch flag
unsigned long lastLickStateChange = 0; // Last time lickometer gone up
unsigned long lastSwitchStateChange = 0; // Last time switch gone up
unsigned long stimStart = 0; // time at which the stim started
unsigned long lastWaterDelivered = 0; 

// Output pulse parameters
const unsigned long waterDur = 20;   // Duration of the water pulse (ms)
const unsigned long airDur = 20;   // Duration of the air pulse (ms)

// Input pulse parameters
volatile bool lickTriggered = false; // Flag to 
volatile bool switchTriggered = false; 

// MATLAB conditional
char serialData; 

bool airpuffDelivered = false;  

void setup() {
Serial.begin(9600);
  pinMode(lickPin, INPUT);
  pinMode(switchPin, INPUT);
  pinMode(waterPin, OUTPUT);
  pinMode(airPin, OUTPUT);

  // Attach the interrupt to the rising edge of the inputPin
  attachInterrupt(digitalPinToInterrupt(lickPin), lickRisingEdgeISR, RISING);
  attachInterrupt(digitalPinToInterrupt(switchPin), switchRisingEdgeISR, RISING);

}

void loop() {
  if (Serial.available() > 0){
    serialData = Serial.read();
    stimStart = millis(); 
    airpuffDelivered = false; 
  }

    // Check if the switch state has changed and deliver water if pushed
    if ((switchTriggered == true) && (millis() - lastSwitchStateChange > 500)) {
       // Pulse detected, trigger the output pulse
       switchTriggered = false;       
       digitalWrite(waterPin, HIGH);
       delay(waterDur);
       digitalWrite(waterPin, LOW);
       lastSwitchStateChange = millis(); 
    }
    
    // Check if the switch state has changed and deliver water if pushed
    if ((lickTriggered == true) && (millis() - lastLickStateChange > 500)) {
       // Pulse detected, trigger the output pulse
       lickTriggered = false;       
       digitalWrite(waterPin, HIGH);
       delay(waterDur);
       digitalWrite(waterPin, LOW);
       lastLickStateChange = millis(); 
    }

    // detect the lick and provide outcome according to the serial command
    //if ((millis() - lastWaterDelivered > 500) && (millis() - stimStart > 200) && (serialData == 'R' || serialData == 'P')) {
      // Check if the lickometer state has changed
    //  if (lickState == HIGH && lastLickStateChange == 0) {
    //    lastLickStateChange = millis(); // Record the time of the lickometer rising edge
    //  } else if (lickState == LOW && lastLickStateChange > 0) {
    //  // Calculate the lick pulse width
    //  unsigned long lickPulseWidth = millis() - lastLickStateChange;
    //  // Check if the lick pulse width is within a valid range (adjust the range as needed)
    //  if (lickPulseWidth > 5 && lickPulseWidth < 500) {
    //    // Pulse detected, trigger the output pulse
    //    if (serialData == 'R') {
    //      digitalWrite(waterPin, HIGH);
    //      delay(waterDur);
    //      digitalWrite(waterPin, LOW);
    //      lastWaterDelivered = millis(); 
    //    } else if ((serialData == 'P') && airpuffDelivered == false) {
    //      digitalWrite(airPin, HIGH);
    //      delay(airDur);
    //      digitalWrite(airPin, LOW);
    //      airpuffDelivered = true;
    //    } 
    //    // Reset the variables for the next pulse detection
    //    lastLickStateChange = 0;
    //  }
    // }
    //}
}

// Interrupt Service Routine (ISR) for the rising edge detection
void lickRisingEdgeISR() {
  // Set the flag to indicate that a pulse should be triggered
  lickTriggered = true;
}

// Interrupt Service Routine (ISR) for the rising edge detection
void switchRisingEdgeISR() {
  // Set the flag to indicate that a pulse should be triggered
  switchTriggered = true;
}
