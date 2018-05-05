const int triggerPin = 2;
const int listenPin = 3;
const int lMotorPin = 10;
const int penPin = 11;
const int rMotorPin = 12;
const int TSpeed = 20;
const int FLSpeed = 20; //Right Wheel
const int FRSpeed = 17; //Left Wheel
const int FLDelay = 2000;
const int FRDelay = 2000;
const int TDelay = 2080;
const int Sleep = 10;
const int MinDistance = 40;
bool isLeft = false;
bool isAtWall = false;
int row = 0;

#include <Servo.h>

Servo lServo;
Servo rServo;
Servo pServo;

void setup() {
  // put your setup code here, to run once:
  pinMode(triggerPin, OUTPUT);
  pinMode(listenPin, INPUT);
  lServo.attach(rMotorPin);
  rServo.attach(lMotorPin);
  pServo.attach(penPin);
  Serial.begin(9600);
  SetPen(true);
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

void TurnOver(){
  lServo.write(90+TSpeed);
  rServo.write(90+TSpeed);
  delay(TDelay);
  lServo.write(90);
  rServo.write(90);
}

void TurnSide(){
  isAtWall = false;
  if(row%2==0){
    lServo.write(90+FLSpeed);
    rServo.write(90);
  }else{
    lServo.write(90);
    rServo.write(90-FRSpeed);
  }
  delay(TDelay);
}

void CoastOut(){
  for(int i=FLSpeed;i>-1;i--){
    lServo.write(90+i);
    rServo.write(90-i);
    delay(10);  
  }
}

void CoastIn(){
   if(isLeft){
    for(int i=0;i<FLSpeed+1;i++){
    lServo.write(90+i);
    delay(10);  
    } 
   }else{
    for(int i=0;i<FRSpeed+1;i++){
    rServo.write(90-i);
    delay(10);  
    } 
   }
}

void Drive(){
  //CoastIn()
  if(isLeft){
    lServo.write(90+FLSpeed);
    rServo.write(90);
    delay(FLDelay);
  }else{
    lServo.write(90);
    rServo.write(90-FRSpeed);
    delay(FRDelay);
  }
  if(GetDistanceToObject(GetTimeObjectAndBack())<MinDistance){
    //isAtWall = true;
  }
  if(isLeft){
    lServo.write(90+FLSpeed);
    rServo.write(90);
    delay(FLDelay);
  }else{
    lServo.write(90);
    rServo.write(90-FRSpeed);
    delay(FRDelay);
  }
  isLeft=!isLeft;
}

void SetPen(bool Down){
  if(!Down){
    pServo.write(90);
  }else{
    pServo.write(180);
  }
}

void loop() {
  if(isAtWall){
    TurnSide();  
    row++;
  }else{
    Drive();  
  }
}
