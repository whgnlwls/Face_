#include "sensorControl.h"
#include "actsocserver.h"

#define buz 21
#define ulst 28
#define ulse 27
#define mot 26

int main() {
	/*
	if (wiringPiSetup() == -1) return -1;
	
	SensorControl sensor = SensorControl(buz, ulst, ulse, mot);
	
	sensor.BZsetBuzzer();
	for(int i = 0; i < 5; i++) {
		sensor.USgetDist();
	}
	sensor.MTsetOpen();
	sensor.MTsetClose();
	*/
	Server server;
	server.createServer(9000);
	
	printf("Finish\n");

	return 0;
}
