#include <Servo.h>

Servo myservo;  // create servo object to control a servo

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
unsigned long stimStart = 0; // time at which the stim started
unsigned long lastWaterDelivered = 0; 
unsigned long lastAirPuffDelivered = 0; 

// Pulse parameters an delays
const unsigned long waterDur = 20;   // Duration of the water pulse (ms)
const unsigned long airDur = 40;   // Duration of the air pulse (ms)
const unsigned long spoutMoveDur = 0;   // Duration of the move (ms)

//Servo parameters
const int outPos = 110;   // out position of the spout
const int inPos = 140;   // long position of the spout
const int servoStepSize = 3; // length of move 
const int servoStepDelay = 15; // delay between moves (in ms)

// MATLAB conditional
char serialData; 
bool newSerialFlag = false; 

bool airpuffDelivered = false;  
bool dropDelivered = false;

void setup() {

Serial.begin(9600);
  pinMode(lickPin, INPUT);
  pinMode(switchPin, INPUT);
  pinMode(waterPin, OUTPUT);
  pinMode(airPin, OUTPUT);

  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  myservo.write(outPos); // tell servo to go to position 100 do not go above 140, 180 gets the engine stuck
  delay(spoutMoveDur);
  
}

void loop() {
  if (Serial.available() > 0){
    serialData = Serial.read();
    stimStart = millis(); 
    airpuffDelivered = false; 
    dropDelivered = false;

    if (serialData == 'S') {    //OUT (short) position for spout, further from the mouse, makes it move fast but poistion guaranteed
      myservo.write(outPos); // tell servo to go to position 140 do not go above 140, 180 gets the engine stuck
      delay(spoutMoveDur);
    }

     if (serialData == 'L') {    //IN (long) position for spout, closer to the mouse, moves fast
      myservo.write(inPos); // tell servo to go to position 140 do not go above 140, 180 gets the engine stuck
      delay(spoutMoveDur);
    }

    if (serialData == 'I') {    //IN position for spout, closer to the mouse, moves slow
      moveServoSmoothly(outPos, inPos, servoStepSize, servoStepDelay);
    }

    if (serialData == 'O') {    //OUT position for spout, further from the mouse, makes it move fast but poistion guaranteed
     moveServoSmoothly(inPos, outPos, servoStepSize, servoStepDelay);  // tell servo to go to position 140 do not go above 140, 180 gets the engine stuck
    }

    if (serialData == 'F') {
      //myservo.write(inPos); // tell servo to go to position 180
      //delay(spoutMoveDur);
      digitalWrite(waterPin, HIGH);
      delay(20000);
      digitalWrite(waterPin, LOW); 
      //myservo.write(outPos); // tell servo to go to position 180
      //delay(spoutMoveDur);     
    }

    if (serialData == 'D') {
      //myservo.write(inPos); // tell servo to go to position 180
      //delay(spoutMoveDur);      
      digitalWrite(waterPin, HIGH);
      delay(waterDur);
      digitalWrite(waterPin, LOW);
      dropDelivered = true;
      //myservo.write(outPos); // tell servo to go to position 180
      //delay(spoutMoveDur);
    }

   if (serialData == 'A') {
      //myservo.write(inPos); // tell servo to go to position 180
      //delay(spoutMoveDur);      
      digitalWrite(airPin, HIGH);
      delay(airDur);
      digitalWrite(airPin, LOW);
      dropDelivered = true;
      //myservo.write(outPos); // tell servo to go to position 180
      //delay(spoutMoveDur);
    }

    if (serialData == 'H') { 
      //myservo.write(inPos); // tell servo to go to position 180
      //delay(spoutMoveDur);       
      generateWaterPulses(); 
      //myservo.write(outPos); // tell servo to go to position 180
      //delay(spoutMoveDur);
    }
  }

  // Poll the input pin to detect the pulse
  int lickState = digitalRead(lickPin);
  int switchState = digitalRead(switchPin);   

    // Check if the switch state has changed and deliver water if pushed
    if (switchState == HIGH && lastSwitchStateChange == 0) {
      lastSwitchStateChange = millis(); // Record the time of the lickometer rising edge
    } else if (switchState == LOW && lastSwitchStateChange > 0) {
      // Calculate the switch pulse width
      unsigned long switchPulseWidth = millis() - lastSwitchStateChange;
      // Check if the switch pulse width is within a valid range (adjust the range as needed)
      if (switchPulseWidth > 5 ) { //&& switchPulseWidth < 500
        // Pulse detected, trigger the output pulse
        digitalWrite(waterPin, HIGH);
        delay(waterDur);
        digitalWrite(waterPin, LOW);
     }
     // Reset the variables for the next pulse detection
     lastSwitchStateChange = 0;
    } 

    // detect the lick and provide outcome according to the serial command
    if  (serialData == 'R' || serialData == 'P') {
      //myservo.write(inPos); // tell servo to go to position 140
      //delay(spoutMoveDur);   
      //make servo move slowly to avoid overheating
      //moveServoSmoothly(outPos, inPos, servoStepSize, servoStepDelay); 
    if ((millis() - lastWaterDelivered > 1000) && (millis() - lastAirPuffDelivered > 1000) ) { // && (millis() - stimStart > 500)
      // Check if the lickometer state has changed
      if (lickState == HIGH && lastLickStateChange == 0) {
        lastLickStateChange = millis(); // Record the time of the lickometer rising edge
      } else if (lickState == LOW && lastLickStateChange > 0) {
      // Calculate the lick pulse width
      unsigned long lickPulseWidth = millis() - lastLickStateChange;
      // Check if the lick pulse width is within a valid range (adjust the range as needed)
      if (lickPulseWidth > 5 ) { //&& lickPulseWidth < 500
        // Pulse detected, trigger the output pulse
        if (serialData == 'R') { // && dropDelivered == false
          digitalWrite(waterPin, HIGH);
          delay(waterDur);
          digitalWrite(waterPin, LOW);
          lastWaterDelivered = millis();
          Serial.println(1);
          //dropDelivered = true;
        } else if ((serialData == 'P') ) { //&& airpuffDelivered == false  
          digitalWrite(airPin, HIGH);
          delay(airDur);
          digitalWrite(airPin, LOW);         
          lastAirPuffDelivered = millis();
          Serial.println(2);  
          //airpuffDelivered = true;
        } 
        // Reset the variables for the next pulse detection
        lastLickStateChange = 0;
      }
     }
    }
}
}

void generateWaterPulses() {
  for (int i = 0; i < 100; i++) {
    digitalWrite(waterPin, HIGH); // Turn on the pulse
    delay(20); // Adjust this value if needed for the desired pulse width (currently set to 5 milliseconds)
    digitalWrite(waterPin, LOW); // Turn off the pulse
    delay(50); // 10-millisecond interval between pulses
  }
}


void moveServoSmoothly(int startPos, int endPos, int stepSize, int stepDelay) {
if (startPos < endPos) {
  for (int pos = startPos; pos <= endPos; pos +=stepSize) { //move in steps of stepSize
    myservo.write(pos);
    delay(stepDelay);
  }
}
else if (startPos > endPos) {
    for (int pos = startPos; pos >= endPos; pos -=stepSize) { //move in steps of stepSize
    myservo.write(pos);
    delay(stepDelay);
  }
}
}
