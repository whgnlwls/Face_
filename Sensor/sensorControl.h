//header guard
#ifndef SENSORCONTROL_H
#define SENSORCONTROL_H

출처: https://boycoding.tistory.com/146 [소년코딩]

//Buzzer
extern void BZsetBuzzer(int buzzer);

//UltraSonic
extern float USgetDist(int ultraSonicTrig, int ultraSonicEcho);

//Motor
extern void MTsetOpen(int motor);
extern void MTsetClose(int motor);

#endif
