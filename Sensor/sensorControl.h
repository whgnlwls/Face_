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
  
  //Buzzer
  void BZsetBuzzer(int buzzer);
  
  //UltraSonic
  float USgetDist(int ultraSonicTrig, int ultraSonicEcho) {return Dist};
  
  //Motor
  void MTsetOpen(int motor);
  void MTsetClose(int motor);
}

#endif
