#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include <softPwm.h>

#define MOT 26

void setup() {
	pinMode(MOT, OUTPUT);
}

void setMotor() {
	softPwmCreate(MOT, 0, 200);
	
	while(1) {
	softPwmWrite(MOT,5);
	delay(3000);
	
	softPwmWrite(MOT,24);
	delay(3000);
	}
}

int main() {
	if(wiringPiSetup() == -1) return -1;
	
	setMotor();
	
	return 0;
}
