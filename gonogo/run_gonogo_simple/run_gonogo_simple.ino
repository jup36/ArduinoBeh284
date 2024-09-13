// Input pin to detect lickometer
const int lickPin = 8; // lickometer
const int switchPin = 4; // water switch
const int waterPin = 2; // water solenoid
const int airPin = 3; // air solenoid

// progression within each trial
int trialStep = 0; 

// Time variables
unsigned long trialStart = 0; 
const unsigned long trialDur = 3000; 
const int waterDur = 20; // 20 ms
const int airDur = 20; 
const int waterTimeOff = 200; // inactivate it for waterTimeOff after each delivery 

// Other task variables
bool repeatWater = true; 

// Variables to store serial data
char serialData; 
bool newSerialFlag = false; 

volatile bool lickDetected = false; // volatile variable to indicate lick detection

void setup() {
  // put your setup code here, to run once:
  pinMode(lickPin, INPUT); 
  pinMode(switchPin, INPUT); 
  pinMode(waterPin, OUTPUT); 
  pinMode(airPin, OUTPUT); 

  digitalWrite(waterPin, LOW); // set the output pins to LOW initially 
  digitalWrite(airPin, LOW); 

  attachInterrupt(digitalPinToInterrupt(lickPin), handleLickPulses, RISING); 

  Serial.begin(9600);  
}

void loop() {
  switch (trialStep) {
    case 0: // Wait for a new serial data to initiate a new trial
      // Check for incoming serial data
      if (Serial.available() > 0) {
        serialData = Serial.read();
        newSerialFlag = true; 
        trialStart = millis();
        trialStep = 1;
      }
    case 1: // 
      if (millis() - trialStart > trialDur) { // if trial timed out
        trialStep = 3;
      } else {
          switch (serialData) {
            case 'W': // A reward trial 'Water'
              if (lickDetected) {
                // deliver water
                digitalWrite(waterPin, HIGH);
                delay(waterDur); 
                digitalWrite(waterPin, LOW);                 
                if (repeatWater) {
                  trialStep = 2; // water recur 
                } else {
                  trialStep = 3; 
                }
              }
            case 'A': // A punishment trial 'Airpuff'
              if (lickDetected) {
                // deliver airpuff
                digitalWrite(airPin, HIGH);
                delay(waterDur); 
                digitalWrite(airPin, LOW); 
                trialStep = 3; // airpuff doesn't recur go to the final step
              }
            case 'O': // An omission trial 'Omission'
              trialStep = 3;      
          } // end of the trial type switch-case
      } // end of if for trial time keeping
    case 2: // wait for the reward inactivation time
      delay(waterTimeOff); 
      trialStep = 1; 
    case 3: // end of a trial
      newSerialFlag = false; 
      trialStep = 0; 
    } // end of the trialStep switch-case
}

// interrupt routine to handle lick detection
void handleLickPulses(){
  // Set the flag to indicate an input pulse was detected
  lickDetected = true;
}
