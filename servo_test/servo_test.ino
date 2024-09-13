#include <Servo.h>

Servo myservo;  // create servo object to control a servo

//Servo parameters
const int outPos = 110;  // out position of the spout
const int inPos = 140;   // long position of the spout
const int servoStepSize = 3; // length of move 
const int servoStepDelay = 15; // delay between moves (in ms)

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  moveServoSmoothly(outPos, inPos, servoStepSize, servoStepDelay);
  //myservo.write(140);   // tell servo to go to position 0
  delay(2000);        // wait 1 second
  moveServoSmoothly(inPos, outPos, servoStepSize, servoStepDelay);  
  //myservo.write(100);  // tell servo to go to position 90
  delay(2000);        // wait 1 second
  //myservo.write(140); // tell servo to go to position 180
  //delay(2000);        // wait 1 second
  //myservo.write(90);  // tell servo to go to position 90
  //delay(2000);        // wait 1 second
  //myservo.write(0);   // tell servo to go to position 0
  //delay(2000);
}

void loop() {
  //myservo.write(0);   // tell servo to go to position 0
  //delay(1000);        // wait 1 second
  //myservo.write(90);  // tell servo to go to position 90
  //delay(1000);        // wait 1 second
  //myservo.write(180); // tell servo to go to position 180
  //delay(1000);        // wait 1 second
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
