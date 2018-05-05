const int triggerPin = 2;
const int listenPin = 3;
const int lMotorPin = 10;
const int rMotorPin = 12;
const int TSpeed = 20;
const int FLSpeed = 45;
const int FRSpeed = 44;
const int FDelay = 500;
const int TDelay = 2080;
const int Sleep = 10;
const int MinDistance = 40;
bool Driving = false;

#include <Servo.h>

Servo lServo;
Servo rServo;

void setup() {
  // put your setup code here, to run once:
  pinMode(triggerPin, OUTPUT);
  pinMode(listenPin, INPUT);
  lServo.attach(rMotorPin);
  rServo.attach(lMotorPin);
  Serial.begin(9600);
}

long GetTimeObjectAndBack(){
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(4);

  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(12);
  digitalWrite(triggerPin, LOW);

  return pulseIn(listenPin, HIGH);
}

int GetDistanceToObject(long TimeTaken){
  return (340.0*(1.0/1000000.0)*(100.0)*(TimeTaken/2.0));  
}

void DriveForward(){
  lServo.write(90+FLSpeed);
  rServo.write(90-FRSpeed);
  delay(10);
}

void Turn(){
  lServo.write(90+TSpeed);
  rServo.write(90+TSpeed);
  delay(TDelay);
  lServo.write(90);
  rServo.write(90);
}

void CoastOut(){
  for(int i=FLSpeed;i>-1;i--){
    lServo.write(90+i);
    rServo.write(90-i);
    delay(10);  
  }
}

void CoastIn(){
   for(int i=0;i<FLSpeed+1;i++){
    lServo.write(90+i);
    rServo.write(90-i);
    delay(10);  
  } 
}

void loop() {
  if(GetDistanceToObject(GetTimeObjectAndBack())<MinDistance){
    Driving = false;
    CoastOut();
    Turn();  
  }else if(!Driving){
    Driving = true;
    CoastIn();
    DriveForward();      
  }
  delay(Sleep);
}
