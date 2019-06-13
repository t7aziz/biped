/*
 * Authours: Taha Aziz and Calvin Tran (code basically all done by Taha)
 * Date: June 12, 2019
 * Description: Culminating robot code for Mr. French's TEJ4M course. 
 * The robot has two motorized arms (and two wheels) which it uses to drag itself along.
 * We were going to only have four legs and no wheels but didn't have enough motors.
 * It uses an ultrasonic sensor to detect walls.
 */
 
#include <Servo.h>

Servo s[10];
int curPos[10] = {0,0,0,0,0,0};    // variable to store the servo positions (not really used in the rest of the code)

const double position[2][10] = {{0, 10, 180, 180, 180, 10}, // Max and min positions for each servo
                                {114, 95, 90, 60, 90, 105}};// Numbers in first row and the down/back position, bottom row is the top/front position
// PING 
int trigPin = 12;    // Trigger
int echoPin = 13;    // Echo
long duration, cm;
 
void setup() {
  // attach each servo to a pin
  s[0].attach(2); 
  s[1].attach(3); 
  s[2].attach(4); 
  s[3].attach(5); 
  s[4].attach(6); 
  s[5].attach(7); 
  
  // PING
  Serial.begin (9600); // serial Port begin
  pinMode(trigPin, OUTPUT); //define inputs and outputs
  pinMode(echoPin, INPUT);
}
       
void loop() {
  walk();
}

void stand(){
  // reference standing position
  swrite(70, 0);
  swrite(0, 1);
  swrite(130, 2);
  swrite(70, 3);
  swrite(0, 4);
  swrite(130, 5);
}

void walk(){
  int del = 500;
  delay(del);
  while(1){
    // simutaneous leg action walk cycle 
    swrite(70, 0);
    swrite(70, 3);
    delay(del);
    swrite(180, 1);
    swrite(180, 4);
    delay(del+350);
    swrite(180, 2);
    swrite(180, 5);
    delay(del);
    swrite(180, 0);
    swrite(180, 3);
    delay(del);
    swrite(0, 1);
    swrite(0, 4);
    delay(100);
    swrite(105, 2);
    swrite(105, 5);
    delay(del);

   while (ping() < 100){
     hop(); // Switched to a different walk cycle close to a wall
   }
  }
}

void hop(){
  // Simutaneous leg action walk cycle with four motors (two don't move)
  int del = 300;
  swrite(180, 0);
  swrite(180, 3);
  delay(del);
  swrite(180, 1);
  swrite(180, 4);
  swrite(180, 2);
  swrite(180, 5);
  delay(del);
  swrite(0, 1);
  swrite(0, 4);
  swrite(40, 2);
  swrite(40, 5);

  unsigned long startTime = millis();
  while (ping() < 40){
    // turns to avoid a wall
    unsigned long currentTime = millis();
    if (startTime - currentTime > 15000){
      // turns right if turning left doesn't distance it from the wall in a certain period of time (15 seconds)
      turn(3, 0);
    }
    else{
      turn(0, 3);
    }
  }
}

void turn(int x, int y){
  // turn(0, 3) turns left and turn(3, 0) turns right
  int del = 500;
  stand();
  
  swrite(70, x);
  swrite(180, y);
  delay(del);
  swrite(180, 1);
  swrite(180, 4);
  delay(del);
  swrite(180, 2);
  swrite(180, 5);
  delay(del);
  swrite(180, x);
  swrite(70, y);
  delay(del);
  swrite(0, 1);
  swrite(0, 4);
  delay(100);
  swrite(90, 2);
  swrite(90, 5);
  delay(del);
  swrite(70, x);
}
  
int ping(){
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  // Read the signal, a HIGH pulse whose duration is the time from the sending of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
 
  // Convert the time into a distance
  cm = (duration/2) / 29.1;
  
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  
  return cm;
}

// Send an angle (p) from 0 to 180 and the motor will move to a position relative to how it was calibrated
void swrite(double p, int n){
  // writes to the motor n and stores the position in curPos
  s[n].write(angle(p, n));
  curPos[n] = p;
}

double angle(double p, int n){
  // takes in an angle from 0 to 180 and finds the respective angle for motor n based on the position's which were calibrated in the const array 
  double i = ((p/180) * abs((position[0][n] - position[1][n])));
  if (position[0][n] > position[1][n]){
    return (position[0][n] - i);
  }else{
    return (position[0][n] + i);
  }
}

void test(){
  // sweeps each pair of motors to test calibration
  
  swrite(0, 0);
  swrite(0, 3);
  delay(3000);
  swrite(180, 0);
  swrite(180, 3);
  delay(5000);

  swrite(0, 1);
  swrite(0, 4);
  delay(3000);
  swrite(180, 1);
  swrite(180, 4);
  delay(5000);

  swrite(0, 2);
  swrite(0, 5);
  delay(3000);
  swrite(180, 2);
  swrite(180, 5);
  delay(5000);
}

