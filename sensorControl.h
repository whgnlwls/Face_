#ifndef SENSORCONTROL_H
#define SENSORCONTROL_H

#include "DoorLockHead.h"

//class
class SensorControl {
private:
	int pinBuzzer;
	int pinUltraSonicTrig;
	int pinUltraSonicEcho;
	int pinMotor;

protected:
	float Dist;

public:
	//initialize
	SensorControl() {};
	SensorControl(int pinBuzzer, int pinUltraSonicTrig, int pinUltraSonicEcho, int pinMotor);

	//get
	float getDist() {
		return Dist;
	}

	//set
	void setDist(float Dist) {
		SensorControl::Dist = Dist;
	}

	//Buzzer
	void BZsetBuzzer();

	//UltraSonic
	void USgetDist();

	//Motor
	void MTsetOpen();
	void MTsetClose();
};

#endif
