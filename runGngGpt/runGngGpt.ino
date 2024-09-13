#include <Servo.h>

Servo myservo;  // create servo object to control a servo

// define input pins 
const int switchPin = 7; // manual switch
const int lickPin = 8;  // lickometer
// define output pins
const int waterPin = 2; // water
const int airPin = 3;   // air
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
  if (Serial.available() > 0) {
    serialData = Serial.read();
    stimStart = millis();

    if (!servoInMotion) { // Only process new commands if the servo is not in motion
      processSerialCommand();
    }
  }

  checkSwitchAndLick();
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
  else if (serialData == 'R' || serialData == 'P') {  // Handle licks with outcomes
    handleLickAndOutcome();
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

void handleLickAndOutcome() {
  // Only move the spout if it's currently out and needs to move in
  if (currentSpoutPos == outPos) {
    moveServoSmoothly(outPos, inPos, servoStepSize, servoStepDelay);  // Move spout IN
    currentSpoutPos = inPos;  // Update the spout's position
  }

  // Check if enough time has passed since the last water or air delivery
  if ((millis() - lastWaterDelivered > 1000) && (millis() - lastAirPuffDelivered > 1000)) {
    int lickState = digitalRead(lickPin);
    // Monitor the lickState on the Serial Monitor
    Serial.print("LickPin State: ");
    if (lickState == HIGH) {
      Serial.println("HIGH (Lick detected)");
    } else {
      Serial.println("LOW (No lick)");
    }

    if (lickState == HIGH && lastLickStateChange == 0) {
      lastLickStateChange = millis();  // Record the lick
    } else if (lickState == LOW && lastLickStateChange > 0) {
      unsigned long lickPulseWidth = millis() - lastLickStateChange;

      if (lickPulseWidth > 5) {  // Valid lick detected
        if (serialData == 'R') {  // Reward water
          setWaterPins(HIGH);
          delay(waterDur);
          setWaterPins(LOW);
          lastWaterDelivered = millis();
          Serial.println(1);  // Feedback for water delivery
        } else if (serialData == 'P') {  // Air puff
          setAirPins(HIGH);
          delay(airDur);
          setAirPins(LOW);
          lastAirPuffDelivered = millis();
          Serial.println(2);  // Feedback for air puff
        }
        lastLickStateChange = 0;  // Reset the lick state for the next detection
      }
    }
  }

  // After the outcome is delivered, you can decide to move the spout out again if needed
  // Uncomment the following code if you want the spout to move out after a lick:
  /*
  if (currentSpoutPos == inPos) {
    moveServoSmoothly(inPos, outPos, servoStepSize, servoStepDelay);  // Move spout OUT
    currentSpoutPos = outPos;  // Update the spout's position
  }
  */
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
