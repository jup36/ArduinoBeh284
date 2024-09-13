const int lickPin = 2; // lickometer
const int switchPin = 3; // water switch
const int waterPin = 5; // water solenoid
const int airPin = 6; // air solenoid
int trialStep = 0; 

volatile bool lickDetected = false; // volatile variable to indicate lick detection

void setup(){
Serial.begin(9600);
  pinMode(lickPin, INPUT); 
  pinMode(switchPin, INPUT); 
  pinMode(waterPin, OUTPUT); 
  pinMode(airPin, OUTPUT); 
  //attachInterrupt(digitalPinToInterrupt(lickPin), handleLickPulses, RISING); 
}


void loop(){
   Serial.println(lickDetected); 
   switch (trialStep) {
    case 0:
        if (lickPin == true) {
          digitalWrite(6, HIGH);
          delay(50); 
          digitalWrite(6, LOW); 
          lickDetected = false;
          trialStep = 1;  
        }
    case 1: 
      digitalWrite(6, LOW);
      delay(50); 
      lickDetected = false;
      trialStep = 0;
   }
}

// interrupt routine to handle lick detection
//void handleLickPulses(){
  // Set the flag to indicate an input pulse was detected
  //lickDetected = true;
//}
