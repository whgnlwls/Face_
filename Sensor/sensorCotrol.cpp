#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <sensorControl.h>

int main() {
  if(wiringPiSetup() == -1) return -1;
  
  return 0;
}
