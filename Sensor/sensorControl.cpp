#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>
#include "sensorControl.h"

//initialize
DoorLock::DoorLock(int pinBuzzer, int pinUltraSonicTrig, int pinUltraSonicEcho, int pinMotor) {
	DoorLock::pinBuzzer = pinBuzzer;
	DoorLock::pinUltraSonicTrig = pinUltraSonicTrig;
	DoorLock::pinUltraSonicEcho = pinUltraSonicEcho;
	DoorLock::pinMotor = pinMotor;

	pinMode(DoorLock::pinBuzzer, OUTPUT);
	pinMode(DoorLock::pinUltraSonicTrig, OUTPUT);
	pinMode(DoorLock::pinUltraSonicEcho, INPUT);
	pinMode(DoorLock::pinMotor, OUTPUT);
}

//Buzzer
void DoorLock::BZsetBuzzer() {
	//NoT = Number of Times
	for (int NoT = 0; NoT < 10; NoT++) {
		for (int frequency = 0; frequency < 100; frequency++) {
			digitalWrite(pinBuzzer, LOW);
			delayMicroseconds(100);
			digitalWrite(pinBuzzer, HIGH);
			delayMicroseconds(100);
		}
		//Blank
		delay(100);
	}
}

//UltraSonic
void DoorLock::USgetDist() {
	float distance;
	int throw_time, catch_time;

	for (int i = 0; i < 5; i++) {
		digitalWrite(pinUltraSonicTrig, LOW);
		delay(200);

		//throw
		digitalWrite(pinUltraSonicTrig, HIGH);
		delayMicroseconds(10);
		digitalWrite(pinUltraSonicTrig, LOW);

		//wait
		while (digitalRead(pinUltraSonicEcho) == 0);

		//if catch signal
		throw_time = micros();
		while (digitalRead(pinUltraSonicEcho) == 1);
		catch_time = micros();
		distance += (catch_time - throw_time) / 29. / 2.;
	}
	setDist(distance / 5.);
	printf("distance %.2fcm\n", getDist());
}

//Motor
void DoorLock::MTsetOpen() {
	softPwmCreate(pinMotor, 0, 200);

	softPwmWrite(pinMotor, 5);
}
void DoorLock::MTsetClose() {
	softPwmCreate(pinMotor, 0, 200);

	softPwmWrite(pinMotor, 24);
}
