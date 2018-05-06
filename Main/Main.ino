// Pin Setting
const int triggerPin = 2;
const int echoPin = 3;
const int lMotorPin = 10;
const int headPin = 11;
const int rMotorPin = 12;

// Forward and Turn Speed Setting
const int TLSpeed = 20;
const int TRSpeed = 13;
const int FLSpeed = 50;
const int FRSpeed = 60;

// Delays For Keeping the Pins High.
const int ForwardDelay = 50;
const int CoastTurnDelay = 8;
const long TurnDelay = 1040;

// Misc Variables.
const int MinDistance = 35; // Min distance to stop when seeing a wall.
const int UncertaintyDistance = 5; // Max uncertainty when finding the distance. ( Used in for sweeping to be sure of the distance ).
const long CenterAngle = 45; // Angle that the head servo is pointing forward.
bool Driving = false; // Tests if car is currently driving.
int foundAngle = -1; // Angle found while sweeping for the furthest distance.

#include <Servo.h>

// Servos/Motors
Servo lServo; //Left Motor.
Servo rServo; //Right Motor.
Servo hServo; //Head Servo.

void setup() {
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  lServo.attach(lMotorPin);
  rServo.attach(rMotorPin);
  hServo.attach(headPin);
  Serial.begin(9600);
}

/*
   Find time to the object and back in microseconds.
*/

long GetTimeObjectAndBack() {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(4);

  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(12);
  digitalWrite(triggerPin, LOW);

  return pulseIn(echoPin, HIGH);
}

/*
   Gets the distance from a time in microseconds (using v=d/t and converting into cm).
*/

int GetDistanceToObject(long TimeTaken) {
  return (340.0 * (1.0 / 1000000.0) * (100.0) * (TimeTaken / 2.0));
}

/*
   Driving function. Drives forwards for ForwardDelay. It then jumps back to the loop to determine the distance. (Also coasts in if it is not driving).
*/

void Drive() {
  if (!Driving) {
    Driving = true;
    CoastIn();
  }
  lServo.write(90 - FLSpeed);
  rServo.write(90 + FRSpeed);
  delay(ForwardDelay);
}

/*
   CoastIn function. Drives in gradually to prevent slipping. Reaches FLSpeed. (Uses CoastTurnDelay to delay the change).
*/

void CoastIn() {
  for (int i = 0; i < FLSpeed + 1; i++) {
    lServo.write(90 - i);
    rServo.write(90 + i);
    delay(CoastTurnDelay);
  }
}

/*
   CoastOut. Drives down gradually to prevent slipping. Reaches speed zero. (Uses CoastTurnDelay to delay the change).
*/

void CoastOut() {
  for (int i = FLSpeed; i > -1; i--) {
    lServo.write(90 - i);
    rServo.write(90 + i);
    delay(CoastTurnDelay);
  }
}

/*
   Looks left, right, and forwards to determine the furthest distance. If it is in a corner, that means the longest distance is the best path.
*/

void Sweep() {
  int maxDistance = -1;
  for (int i = 135; i > -46; i -= 90) {
    if (i < 0)i = 0;
    hServo.write(i);
    delay(100);
    int Distance = GetDistanceToObject(GetTimeObjectAndBack());
    int Distance2 = 0;
    bool foundDistance = false;

    do {
      delay(500);
      Distance2 = GetDistanceToObject(GetTimeObjectAndBack());
      if (abs(Distance - Distance2) < UncertaintyDistance) {
        foundDistance = true;
      } else {
        Distance = Distance2;
      }
    } while (!foundDistance);

    if (maxDistance < Distance || maxDistance == -1) {
      foundAngle = i;
      maxDistance = Distance;
    }
    delay(1000);
  }

  hServo.write(CenterAngle);
}

/*
   Turns the car acording to the angle found in the Sweep function.
*/

void Turn() {
  if (foundAngle > CenterAngle) {
    lServo.write(90 + TLSpeed);
    rServo.write(90 + TLSpeed);
  } else if (foundAngle < CenterAngle) {
    lServo.write(90 - TRSpeed);
    rServo.write(90 - TRSpeed);
  }
  delay(TurnDelay);
}

void loop() {
  Sweep();
  Turn();
  do {
    Drive();
  } while (GetDistanceToObject(GetTimeObjectAndBack()) > MinDistance);
  CoastOut();
  Driving = false;
}
