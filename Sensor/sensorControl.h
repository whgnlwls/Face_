//header guard
#ifndef SENSORCONTROL_H
#define SENSORCONTROL_H

//class
class DoorLock {
  private:
  int pinBuzzer;
  int pinUltraSonicTrig;
  int pinUltraSonicEcho;
  int pinMotor;
  
  protected:
  float Dist;
  
  public:
  //initialize
  DoorLock(int pinBuzzer, int pinUltraSonicTrig, int pinUltraSonicEcho, int pinMotor);
  
  //get
  getDist();
  
  //set
  setDist(float Dist);
  
  //Buzzer
  void BZsetBuzzer(int buzzer);
  
  //UltraSonic
  void USgetDist(int ultraSonicTrig, int ultraSonicEcho);
  
  //Motor
  void MTsetOpen(int motor);
  void MTsetClose(int motor);
}

#endif
