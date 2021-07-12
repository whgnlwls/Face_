//header guard
#ifndef SENSORCONTROL_H
#define SENSORCONTROL_H

출처: https://boycoding.tistory.com/146 [소년코딩]

//Buzzer
void BZsetBuzzer(int buzzer);

//UltraSonic
float USgetDist(int ultraSonicTrig, int ultraSonicEcho);

//Motor
void MTsetOpen(int motor);
void MTsetClose(int motor);

#endif
