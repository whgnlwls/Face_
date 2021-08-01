#include "server.h"

#define buz 21
#define ulst 28
#define ulse 27
#define mot 26

int main() {
	if (wiringPiSetup() == -1) return -1;
	
	Server server;
	
	server.SensorControl(buz, ulst, ulse, mot);
	
	server.BZsetBuzzer();
	for(int i = 0; i < 5; i++) {
		server.USgetDist();
	}
	server.MTsetOpen();
	server.MTsetClose();
	
	Server server;
	server.createServer(9000);
	
	printf("Finish\n");

	return 0;
}
