#ifndef SERVER_H
#define SERVER_H

#include "DoorLockHead.h"

#define BUFSIZE 512

//class
class Server {
private:
	int serverSocket;
	int clientSocket;
	
	struct sockaddr_in serverAddr;
	struct sockaddr_in clientAddr;

protected:
	
public:
	//initialize
	Server() {};
	Server(int PORT);
	
	~Server();
	
	//bind
	void bindSocket();
	
	//listen
	void listenSocket();
	
	//accept
	void acceptSocket();
	
	//thread
	static void* loginThread(void* clientSock);
	static void* functionThread(void* clientSock);

	//print error
	void showError(const char* msg);
	
	//create server
	void createServer(int PORT);
};

#endif
