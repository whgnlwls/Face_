//include main method if(wiringPiSetup() == -1) return -1;
#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <sensorControl.h>

//Buzzer
void BZsetBuzzer(int buzzer) {
	//NoT = Number of Times
	for(int NoT = 0; NoT < 10; NoT++) {
		for(int frequency = 0; frequency < 100; frequency++) {
		digitalWrite(buzzer, LOW);
		delay(2);
		digitalWrite(buzzer, HIGH);
		delay(2);
		}
		//Blank
		delay(100);
	}
}
//UltraSonic
float USgetDist(int ultraSonicTrig, int ultraSonicEcho) {
	float distance;
	int throw_time, catch_time;
	
	for(int i = 0; i < 5; i++) {
    digitalWrite(ultraSonicTrig, LOW);
    delay(200);
    
    //throw
    digitalWrite(ultraSonicTrig, HIGH);
    delayMicroseconds(10);
    digitalWrite(ultraSonicTrig, LOW);
    
    //wait
    while (digitalRead(ultraSonicEcho) == 0);
    
    //if catch signal
    throw_time = micros();
    while (digitalRead(ultraSonicEcho) == 1);
    catch_time = micros();
    
    distance += (catch_time - throw_time) / 29. / 2.;
  }
  distance = distance / 5;
  printf("distance %.2fcm\n", distance);
  
  return distance;
}
//Motor
void MTsetOpen(int motor) {
	softPwmCreate(motor, 0, 200);

	softPwmWrite(motor,5);
}
void MTsetClose(int motor) {
	softPwmCreate(motor, 0, 200);
	
	softPwmWrite(motor,24);
}
