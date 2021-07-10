#include <stdio.h>
#include <wiringPi.h>

#define BUZ 21

void setup() {
	pinMode(BUZ, OUTPUT);
}

void setBuzzer() {
	while(1) {
		digitalWrite(BUZ, LOW);
		delay(2);
		digitalWrite(BUZ, HIGH);
		delay(2);
	}
}

int main() {
	if(wiringPiSetup() == -1) return -1;
	
	setup();
	setBuzzer();
	
	return 0;
}
