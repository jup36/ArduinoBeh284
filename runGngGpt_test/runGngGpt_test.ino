#include <Servo.h>

Servo myservo;  // create servo object to control a servo

// define input pins 
const int switchPin = 7; // manual switch
const int lickPin = 8;   // lickometer
// define output pins
const int waterPin = 2;  // water
const int airPin = 3;    // air
const int waterStrobe = 4; // this mirrors waterPin (strobe to NP nidq)
const int airStrobe = 5; // this mirrors airPin (strobe to NP nidq)
const int manualWaterStrobe = 6; // strobe for manual water delivery

// Variables
unsigned long lastLickStateChange = 0;
unsigned long lastSwitchStateChange = 0;
unsigned long stimStart = 0;
unsigned long lastWaterDelivered = 0; 
unsigned long lastAirPuffDelivered = 0; 

// Pulse parameters and delays
const unsigned long waterDur = 20;   // Duration of the water pulse (ms)
const unsigned long airDur = 40;     // Duration of the air pulse (ms)
const unsigned long spoutMoveDur = 0;

// Servo parameters
const int outPos = 110;  // out position of the spout
const int inPos = 140;   // in position of the spout
const int servoStepSize = 5; 
const int servoStepDelay = 15; 
int currentSpoutPos = outPos;  // Start with the spout in the 'out' position

// Track the last command received ('R' for water, 'P' for air)
char lastCommand = ' ';  

// MATLAB conditional
char serialData;
bool newSerialFlag = false; 
bool servoInMotion = false;  // Track if the servo is in motion

void setup() {
  Serial.begin(9600);
  pinMode(lickPin, INPUT);
  pinMode(switchPin, INPUT);
  pinMode(waterPin, OUTPUT);
  pinMode(airPin, OUTPUT);
  
  myservo.attach(9); // use digital 9 channel for servo control 
  myservo.write(outPos);  // Initial position
  delay(spoutMoveDur);
}

void loop() {
  // Handle serial data for commands
  if (Serial.available() > 0) {
    serialData = Serial.read();
    stimStart = millis();

    // Track the last command (R for water, P for air)
    if (serialData == 'R' || serialData == 'P') {
      lastCommand = serialData;
      // Ensure spout moves to 'inPos' if it isn't already there
      if (currentSpoutPos == outPos) {
        moveServoSmoothly(outPos, inPos, servoStepSize, servoStepDelay);
        currentSpoutPos = inPos;  // Update to 'inPos'
        Serial.println("Spout moved to IN position.");
      }
    }

    // Process other serial commands (servo, water, air)
    if (!servoInMotion) { // Only process new commands if the servo is not in motion
      processSerialCommand();
    }
  }

  // Continuously monitor licks
  handleLickDetection();

  // Check the switch as well
  checkSwitchAndLick();
}

void handleLickDetection() {
  int lickState = digitalRead(lickPin);  // Read the lickPin state
  // Monitor the lickState on the Serial Monitor
  Serial.print("LickPin State: ");
  if (lickState == HIGH) {
    Serial.println("HIGH (Lick detected)");
  } else {
    Serial.println("LOW (No lick)");
  }

  // Check if enough time has passed since the last water or air delivery
  if ((millis() - lastWaterDelivered > 1000) && (millis() - lastAirPuffDelivered > 1000)) {
    // If a lick is detected, handle the outcome based on the last command
    if (lickState == HIGH && lastLickStateChange == 0) {
      lastLickStateChange = millis();  // Record the lick
      
      if (lastCommand == 'R') {  // Water reward if last command was 'R'
        setWaterPins(HIGH);
        delay(waterDur);
        setWaterPins(LOW);
        lastWaterDelivered = millis();
        Serial.println("Water delivered.");
      } 
      else if (lastCommand == 'P') {  // Air puff if last command was 'P'
        setAirPins(HIGH);
        delay(airDur);
        setAirPins(LOW);
        lastAirPuffDelivered = millis();
        Serial.println("Air puff delivered.");
      }
    } 
    else if (lickState == LOW && lastLickStateChange > 0) {
      lastLickStateChange = 0;  // Reset the lick state for the next detection
    }
  }
}

void processSerialCommand() {
  if (serialData == 'S') {    // OUT position for spout, fast move
    // Only move the spout if it is currently in the 'in' position
    if (currentSpoutPos == inPos) {
      moveServoSmoothly(inPos, outPos, servoStepSize, servoStepDelay);
      currentSpoutPos = outPos;  // Update the current position to 'out'
    } else {
      Serial.println("Spout already in OUT position, no movement needed.");
    }
  }
  else if (serialData == 'L') {    // IN position for spout, fast move
    if (currentSpoutPos == outPos) {
      myservo.write(inPos);
      currentSpoutPos = inPos;  // Update the current position to 'in'
    } else {
      Serial.println("Spout already in IN position, no movement needed.");
    }
  }
  else if (serialData == 'I') {    // IN position for spout, slow move
    if (currentSpoutPos == outPos) {
      moveServoSmoothly(outPos, inPos, servoStepSize, servoStepDelay);
      currentSpoutPos = inPos;  // Update the current position to 'in'
    } else {
      Serial.println("Spout already in IN position, no movement needed.");
    }
  }
  else if (serialData == 'O') {    // OUT position for spout, slow move
    if (currentSpoutPos == inPos) {
      moveServoSmoothly(inPos, outPos, servoStepSize, servoStepDelay);
      currentSpoutPos = outPos;  // Update the current position to 'out'
    } else {
      Serial.println("Spout already in OUT position, no movement needed.");
    }
  }
  else if (serialData == 'F') {    // Long water pulse
    triggerWater(20000);
  }
  else if (serialData == 'D') {    // Short water pulse
    triggerWater(waterDur);
  }
  else if (serialData == 'A') {    // Air pulse
    triggerAir(airDur);
  }
  else if (serialData == 'H') {    // Generate water pulses
    generateWaterPulses();
  }
}

void moveServoSmoothly(int startPos, int endPos, int stepSize, int stepDelay) {
  servoInMotion = true;  // Mark the servo as moving
  if (startPos < endPos) {
    for (int pos = startPos; pos <= endPos; pos += stepSize) {
      myservo.write(pos);
      delay(stepDelay);
    }
  } else {
    for (int pos = startPos; pos >= endPos; pos -= stepSize) {
      myservo.write(pos);
      delay(stepDelay);
    }
  }
  servoInMotion = false;  // Mark the servo as no longer moving
}

void triggerWater(unsigned long duration) {
  digitalWrite(waterPin, HIGH);
  delay(duration);
  digitalWrite(waterPin, LOW);
}

void triggerAir(unsigned long duration) {
  digitalWrite(airPin, HIGH);
  delay(duration);
  digitalWrite(airPin, LOW);
}

void generateWaterPulses() {
  for (int i = 0; i < 100; i++) {
    digitalWrite(waterPin, HIGH);
    delay(20);
    digitalWrite(waterPin, LOW);
    delay(50);
  }
}

void checkSwitchAndLick() {
  //int lickState = digitalRead(lickPin);
  int switchState = digitalRead(switchPin);   

  if (switchState == HIGH && lastSwitchStateChange == 0) {
    lastSwitchStateChange = millis();
  } else if (switchState == LOW && lastSwitchStateChange > 0) {
    unsigned long switchPulseWidth = millis() - lastSwitchStateChange;
    if (switchPulseWidth > 5) {
      setManualWaterPins(HIGH); 
      delay(waterDur);
      setManualWaterPins(LOW); 
    }
    lastSwitchStateChange = 0;
  }
}

void setWaterPins(int state) {
  digitalWrite(waterPin, state);
  digitalWrite(waterStrobe, state);
}

void setAirPins(int state) {
  digitalWrite(airPin, state);
  digitalWrite(airStrobe, state);
}

void setManualWaterPins(int state) {
  digitalWrite(waterPin, state);
  digitalWrite(manualWaterStrobe, state);
}
