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

protected:
	
public:
	//initialize
	Server() {};
	Server(int pinBuzzer, int pinUltraSonicTrig, int pinUltraSonicEcho, int pinMotor, int PORT);
	~Server();
	
	//bind
	void bindSocket();
	
	//listen
	void listenSocket();
	
	//accept
	void acceptSocket();
	
	//thread
	static void* clientThread(void* client);

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
