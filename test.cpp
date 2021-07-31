#include "server.h"
#include "sensorControl.h"

#define buz 21
#define ulst 28
#define ulse 27
#define mot 26

int main() {
	if (wiringPiSetup() == -1) return -1;

	SensorControl sensorControl = SensorControl(buz, ulst, ulse, mot);
	
	sensorControl.BZsetBuzzer();
	for(int i = 0; i < 5; i++) {
		sensorControl.USgetDist();
	}
	sensorControl.MTsetOpen();
	sensorControl.MTsetClose();
	
	Server server = Server(9000);
	server.bindSocket();
	server.listenSocket();
	server.acceptSocket();
	
	printf("Finish\n");

	return 0;
}
