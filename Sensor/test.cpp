#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>
#include "sensorControl.h"

#define buz
#define ulst
#define ulse
#define mot

int main() {
  include main method if(wiringPiSetup() == -1) return -1;
  
  DoorLock doorlock = new DoorLock(buz, ulst, ulse, mot);
  
  doorlock.BZsetBuzzer();
  doorlock.USgetDist();
  doorlock.MTsetOpen();
  doorlock.MTsetClose();
  
  return 0;
}
