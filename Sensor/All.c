#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include <softPwm.h>

#define MOT 26
#define BUZ 21
#define US1_trig 28
#define US1_echo 27

//setting
void setMotor() {
	softPwmCreate(MOT, 0, 200);
	
	softPwmWrite(MOT,5);
	delay(3000);
	
	softPwmWrite(MOT,24);
	delay(3000);
}

void setBuzzer() {
		digitalWrite(BUZ, LOW);
		delay(2);
		digitalWrite(BUZ, HIGH);
		delay(2);
}

float getDist() {
	float distance;
	int throw_time, catch_time;
	
	digitalWrite(US1_trig, LOW);
    delay(500);
    digitalWrite(US1_trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(US1_trig, LOW);
    
    while (digitalRead(US1_echo) == 0);
    
    throw_time = micros();
    while (digitalRead(US1_echo) == 1);
    catch_time = micros();
    
    distance = (catch_time - throw_time) / 29. / 2.;
    printf("distance %.2f cm\n", distance);
}

//function
void setup() {
	pinMode(MOT, OUTPUT);
	pinMode(BUZ, OUTPUT);
	pinMode(US1_trig, OUTPUT) ;
	pinMode(US1_echo , INPUT) ;
}

void motFunc() {
	setMotor();
}

void buzFunc() {
	for(int i = 0; i < 10; i++) {
		for(int w = 0; w < 100; w++) {
			setBuzzer();
		}
		delay(100);
	}
}

void us1Func() {
	getDist();
}

//main
int main() {
	if(wiringPiSetup() == -1) return -1;
	
	setup();
	
	for(;;) {
	motFunc();
	buzFunc();
	for(int i = 0; i < 10; i++) {
		us1Func();
		}
	}
	
	return 0;
}
