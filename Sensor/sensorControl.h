void buzFunc() {
	for(int i = 0; i < 10; i++) {
		for(int w = 0; w < 100; w++) {
			setBuzzer();
		}
		delay(100);
	}
}

float USgetDist(int ultraSonicTrig, int ultraSonicEcho) {
	float distance;
	int throw_time, catch_time;
	
	for(int i = 0; i < 5; i++) {
    digitalWrite(ultraSonicTrig, LOW);
    delay(200);
    
    //throw
    digitalWrite(ultraSonicTrig, HIGH);
    delayMicroseconds(10);
    digitalWrite(ultraSonicTrig, LOW);
    
    //wait
    while (digitalRead(ultraSonicEcho) == 0);
    
    //if catch signal
    throw_time = micros();
    while (digitalRead(ultraSonicEcho) == 1);
    catch_time = micros();
    
    distance += (catch_time - throw_time) / 29. / 2.;
  }
  distance = distance / 5;
  printf("distance %.2fcm\n", distance);
  
  return distance;
}
