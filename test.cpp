#include "sensorControl.h"
#include "server.h"

#define buz 21
#define ulst 28
#define ulse 27
#define mot 26

int main() {
	if (wiringPiSetup() == -1) return -1;
	/*
	sensorControl.BZsetBuzzer();
	for(int i = 0; i < 5; i++) {
		sensorControl.USgetDist();
	}
	sensorControl.MTsetOpen();
	sensorControl.MTsetClose();
	*/
	SensorControl sensor = SensorControl(buz, ulst, ulse, mot);
	Server server = Server(9000);
	
	printf("Finish\n");

	return 0;
}
