#include "server.h"

#define buz 21
#define fulst 28
#define fulse 27
#define bulst 22
#define bulse 23
#define mot 26

int main() {
	if (wiringPiSetup() == -1) return -1;
	
	Server server(buz, fulst, fulse, bulst, bulse, mot, 9000);
	printf("Finish\n");

	return 0;
}
