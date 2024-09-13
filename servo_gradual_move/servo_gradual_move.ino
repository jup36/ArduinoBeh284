#include <Servo.h>

 

Servo myservo;  // create servo object to control a servo

 

void setup() {

  myservo.attach(9);  // attaches the servo on pin 9 to the servo object

}

 

void loop() {

  gradualMove(0, 90, 10);  // Move from 0 to 90 degrees, step by 15

  delay(1000);             // wait 1 second

  gradualMove(90, 180, 10); // Move from 90 to 180 degrees, step by 15

  delay(1000);              // wait 1 second

  gradualMove(180, 90, 10); // Move from 180 back to 90 degrees, step by 15

  while(1);                 // Stop any further movement

}

 

void gradualMove(int startPos, int endPos, int stepDelay) {

  if (startPos < endPos) {

    for (int pos = startPos; pos <= endPos; pos++) {

      myservo.write(pos);            // Move to the next position

      delay(stepDelay);              // Short delay to slow down the movement

    }

  } else {

    for (int pos = startPos; pos >= endPos; pos--) {

      myservo.write(pos);

      delay(stepDelay);

    }

  }

}
