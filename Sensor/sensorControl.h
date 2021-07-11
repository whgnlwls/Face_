#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include <softPwm.h>

//Buzzer
void BZsetBuzzer(int buzzer);

//UltraSonic
float USgetDist(int ultraSonicTrig, int ultraSonicEcho);

//Motor
void MTsetOpen(int motor);
void MTsetClose(int motor);
