const int triggerPin = 2;
const int listenPin = 3;
const int lMotorPin = 10;
const int rMotorPin = 12;
const int headPin = 11;
const int TSpeed = 20;
const int FLSpeed = 50;//Right wheel
const int FRSpeed = 60;//Left wheel
const int FDelay = 50;
const long TDelay = 1040;
const int Sleep = 10;
const int MinDistance = 35;
const int CDistance = 5;
const long CenterAngle = 45;
bool Driving = false;
int foundAngle = -1;

#include <Servo.h>

Servo lServo;
Servo rServo;
Servo hServo;

void setup() {
  // put your setup code here, to run once:
  pinMode(triggerPin, OUTPUT);
  pinMode(listenPin, INPUT);
  lServo.attach(rMotorPin);
  rServo.attach(lMotorPin);
  hServo.attach(headPin);
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

void Drive(){
  if(!Driving){
    Driving = true;
    CoastIn();
  }
  lServo.write(90-FLSpeed);
  rServo.write(90+FRSpeed);
  delay(FDelay);
}

void CoastOut(){
  for(int i=FLSpeed;i>-1;i--){
    lServo.write(90-i);
    rServo.write(90+i);
    delay(8);  
  }
}

void CoastIn(){
   for(int i=0;i<FLSpeed+1;i++){
    lServo.write(90-i);
    rServo.write(90+i);
    delay(8);  
  } 
}

void Sweep(){
  int maxDistance = -1;
   for(int i=135;i>-46;i-=90){
    if(i<0)i=0;
    hServo.write(i);
    delay(100);
    int Distance = GetDistanceToObject(GetTimeObjectAndBack());
    int Distance2 = 0;
    bool Test = false;
    
    do{
      delay(500);
      Distance2 = GetDistanceToObject(GetTimeObjectAndBack());
      if(abs(Distance-Distance2)<CDistance){
        Test = true;
      }else{
        Distance = Distance2;  
      }
    }while(Test==false);
    
    if(maxDistance<Distance||maxDistance==-1){
      foundAngle = i;
      maxDistance = Distance;
    }
    delay(1000); 
   }   
}

void TurnMaze(){
  hServo.write(CenterAngle);
      if(foundAngle>CenterAngle){
        lServo.write(90+TSpeed);
        rServo.write(90+TSpeed);
      }else if(foundAngle<CenterAngle){
        lServo.write(90-13);
        rServo.write(90-13);
      }
      delay(TDelay);
}

//void TurnDog(){
  //   hServo.write(CenterAngle);
    //long travelAngle = abs(CenterAngle-foundAngle);
    //if(foundAngle>CenterAngle){
      //lServo.write(90-TSpeed);
     // rServo.write(90-TSpeed);
    //}else{
      //lServo.write(90+TSpeed);
      //rServo.write(90+TSpeed);
   // }
    //delay((int)((travelAngle*TDelay)/90.0));
//}

void loop() {
  Sweep();
  TurnMaze();
  do{
  Drive();
  }while(GetDistanceToObject(GetTimeObjectAndBack())>MinDistance);
  CoastOut();
  Driving = false;
}
