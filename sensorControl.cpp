#include "sensorControl.h"

//initialize
SensorControl::SensorControl(int pinBuzzer, int pinUltraSonicTrig, int pinUltraSonicEcho, int pinMotor) {
	SensorControl::pinBuzzer = pinBuzzer;
	SensorControl::pinUltraSonicTrig = pinUltraSonicTrig;
	SensorControl::pinUltraSonicEcho = pinUltraSonicEcho;
	SensorControl::pinMotor = pinMotor;

	pinMode(SensorControl::pinBuzzer, OUTPUT);
	pinMode(SensorControl::pinUltraSonicTrig, OUTPUT);
	pinMode(SensorControl::pinUltraSonicEcho, INPUT);
	pinMode(SensorControl::pinMotor, OUTPUT);

	printf("pinNum : buz %d, ulst %d, ulse %d, mot %d\n", pinBuzzer, pinUltraSonicTrig, pinUltraSonicEcho, pinMotor);
}

//Buzzer
void SensorControl::BZsetBuzzer() {
	//NoT = Number of Times
	for (int NoT = 0; NoT < 10; NoT++) {
		for (int frequency = 0; frequency < 100; frequency++) {
			digitalWrite(pinBuzzer, LOW);
			delayMicroseconds(1000);
			digitalWrite(pinBuzzer, HIGH);
			delayMicroseconds(1000);
		}
		//Blank
		delay(100);
	}
}

//UltraSonic
void SensorControl::USgetDist() {
	float distance = 0;
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
void SensorControl::MTsetOpen() {
	softPwmCreate(pinMotor, 0, 200);

	softPwmWrite(pinMotor, 5);
	delay(1000);
}
void SensorControl::MTsetClose() {
	softPwmCreate(pinMotor, 0, 200);

	softPwmWrite(pinMotor, 24);
	delay(1000);
}
