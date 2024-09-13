int trialStep = 0;
// Input pin to detect lickometer
const int lickPin = 8; // lickometer
const int switchPin = 4; // water switch
const int waterPin = 2; // water solenoid
const int airPin = 3; // air solenoid


char serialData; 
bool newSerialFlag = false; 

volatile bool lickDetected = false; // volatile variable to indicate lick detection

void setup(){
Serial.begin(9600);
  pinMode(lickPin, INPUT); 
  pinMode(switchPin, INPUT); 
  pinMode(waterPin, OUTPUT); 
  pinMode(airPin, OUTPUT);  
}


void loop(){
      if (Serial.available() > 0){
        serialData = Serial.read();
        if (serialData == 'R') { 
            digitalWrite(waterPin, HIGH);
            delay(50); 
            digitalWrite(waterPin, LOW);
        } else if (serialData == 'P') { 
            digitalWrite(airPin, HIGH);
            delay(50); 
            digitalWrite(airPin, LOW);
        }
      }
}
