int trialStep = 0;
// Input pin to detect lickometer
const int lickPin = 8; // lickometer
const int switchPin = 4; // water switch
const int waterPin = 2; // water solenoid
const int airPin = 3; // air solenoid


char serialData; 
bool newSerialFlag = false; 

// Variables
//bool switchDetected = false; // switch flag
unsigned long lastLickStateChange = 0; // Last time lickometer gone up
unsigned long lastSwitchStateChange = 0; // Last time switch gone up

//volatile bool lickDetected = false; // volatile variable to indicate lick detection

// Pulse parameters
const unsigned long waterDur = 20;   // Duration of the water pulse (ms)
const unsigned long airDur = 20;   // Duration of the air pulse (ms)



void setup(){
Serial.begin(9600);
  pinMode(lickPin, INPUT); 
  pinMode(switchPin, INPUT); 
  pinMode(waterPin, OUTPUT); 
  pinMode(airPin, OUTPUT);  
}


void loop(){
  // Poll the input pin to detect the pulse
  int lickState = digitalRead(lickPin);
  int switchState = digitalRead(switchPin); 
      
      if (Serial.available() > 0){
        serialData = Serial.read();
        if (serialData == 'R') { 
            digitalWrite(waterPin, HIGH);
            delay(50); 
            digitalWrite(waterPin, LOW);
        } else if (serialData == 'P') { 
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
            //digitalWrite(airPin, HIGH);
            //delay(50); 
            //digitalWrite(airPin, LOW);
        }
      }
}
