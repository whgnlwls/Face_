//header guard
#ifndef SENSORCONTROL_H
#define SENSORCONTROL_H

//class
class DoorLock {
private:
	int pinBuzzer;
	int pinUltraSonicTrig;
	int pinUltraSonicEcho;
	int pinMotor;

protected:
	float Dist;

public:
	//initialize
	DoorLock(int pinBuzzer, int pinUltraSonicTrig, int pinUltraSonicEcho, int pinMotor);

	//get
	float getDist() {
		return Dist;
	}

	//set
	void setDist(float Dist) {
		DoorLock::Dist = Dist;
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
