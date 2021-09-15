#include "sensorControl.h"

//initialize
SensorControl::SensorControl(int pinBuzzer, int F_pinUltraSonicTrig, int F_pinUltraSonicEcho, int B_pinUltraSonicTrig, int B_pinUltraSonicEcho, int pinMotor) {
	SensorControl::pinBuzzer = pinBuzzer;
	SensorControl::F_pinUltraSonicTrig = F_pinUltraSonicTrig;
	SensorControl::F_pinUltraSonicEcho = F_pinUltraSonicEcho;
	SensorControl::B_pinUltraSonicTrig = B_pinUltraSonicTrig;
	SensorControl::B_pinUltraSonicEcho = B_pinUltraSonicEcho;
	SensorControl::pinMotor = pinMotor;

	pinMode(SensorControl::pinBuzzer, OUTPUT);
	pinMode(SensorControl::F_pinUltraSonicTrig, OUTPUT);
	pinMode(SensorControl::F_pinUltraSonicEcho, INPUT);
	pinMode(SensorControl::B_pinUltraSonicTrig, OUTPUT);
	pinMode(SensorControl::B_pinUltraSonicEcho, INPUT);
	pinMode(SensorControl::pinMotor, OUTPUT);

	printf("pinNum : buz %d, fulst %d, fulse %d, bulst %d, bulse %d, mot %d\n", pinBuzzer, F_pinUltraSonicTrig, F_pinUltraSonicEcho, B_pinUltraSonicTrig, B_pinUltraSonicEcho, pinMotor);
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
void SensorControl::F_USgetDist() {
	float distance = 0;
	int throw_time, catch_time;

	for (int i = 0; i < 5; i++) {
		digitalWrite(F_pinUltraSonicTrig, LOW);
		delay(200);

		//throw
		digitalWrite(F_pinUltraSonicTrig, HIGH);
		delayMicroseconds(10);
		digitalWrite(F_pinUltraSonicTrig, LOW);

		//wait
		while (digitalRead(F_pinUltraSonicEcho) == 0);

		//if catch signal
		throw_time = micros();
		while (digitalRead(F_pinUltraSonicEcho) == 1);
		catch_time = micros();
		distance += (catch_time - throw_time) / 29. / 2.;
	}
	F_setDist(distance / 5.);
	printf("distance FUS %.2fcm\n", F_getDist());
}
void SensorControl::B_USgetDist() {
	float distance = 0;
	int throw_time, catch_time;

	for (int i = 0; i < 5; i++) {
		digitalWrite(B_pinUltraSonicTrig, LOW);
		delay(200);

		//throw
		digitalWrite(B_pinUltraSonicTrig, HIGH);
		delayMicroseconds(10);
		digitalWrite(B_pinUltraSonicTrig, LOW);

		//wait
		while (digitalRead(B_pinUltraSonicEcho) == 0);

		//if catch signal
		throw_time = micros();
		while (digitalRead(B_pinUltraSonicEcho) == 1);
		catch_time = micros();
		distance += (catch_time - throw_time) / 29. / 2.;
	}
	B_setDist(distance / 5.);
	printf("distance BUS %.2fcm\n", B_getDist());
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
