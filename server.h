#ifndef SERVER_H
#define SERVER_H

#include "sensorControl.h"

#define BUFSIZE 512

//class
class Server : public SensorControl {
private:
	int serverSocket;
	int clientSocket;
	
	struct sockaddr_in serverAddr;
	struct sockaddr_in clientAddr;
	int clientAddrlen;
	
	string accountfilePath;
	string openCV_ID;
	string openCV_Confidence;

protected:
	
public:
	//initialize
	Server() {};
	Server(int pinBuzzer, int F_pinUltraSonicTrig, int F_pinUltraSonicEcho, int B_pinUltraSonicTrig, int B_pinUltraSonicEcho, int pinMotor, int PORT);
	~Server();
	
	//bind
	void bindSocket();
	
	//listen
	void listenSocket();
	
	//accept
	void acceptSocket();
	
	//thread
	static void* clientThread(void* client);
	static void* doorlockThread(void* pClass);

	//print error
	void showError(const char* msg);
	
	//create server
	void createServer();
};

//type define
typedef struct ClientControl {
	Server* CLCR_pClass;
	int CLCR_Socket;
}CLCR;

#endif
