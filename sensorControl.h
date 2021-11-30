#ifndef SENSORCONTROL_H
#define SENSORCONTROL_H

#include "DoorLockHead.h"

#define F_US_STD_DIST 30
#define B_US_STD_DIST 5

//class
class SensorControl {
private:
	int pinBuzzer;
	int F_pinUltraSonicTrig;
	int F_pinUltraSonicEcho;
	int B_pinUltraSonicTrig;
	int B_pinUltraSonicEcho;
	int pinMotor;

protected:
	float F_Dist;
	float B_Dist;
	
	int isCameraUse;
	int isMotorUse;
	int isBuzzerUse;
	int isFUltraSonicUse;
	int isBUltraSonicUse;

public:
	//initialize
	SensorControl() {};
	SensorControl(int pinBuzzer, int F_pinUltraSonicTrig, int F_pinUltraSonicEcho, int B_pinUltraSonicTrig, int B_pinUltraSonicEcho, int pinMotor);

	//get
	float F_getDist() {
		return F_Dist;
	}
	float B_getDist() {
		return B_Dist;
	}

	//set
	void F_setDist(float Dist) {
		SensorControl::F_Dist = Dist;
	}
	void B_setDist(float Dist) {
		SensorControl::B_Dist = Dist;
	}

	//Buzzer
	void BZsetBuzzer();

	//UltraSonic
	void F_USgetDist();
	void B_USgetDist();

	//Motor
	void MTsetOpen();
	void MTsetClose();
};

#endif
