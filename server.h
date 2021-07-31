#ifndef SERVER_H
#define SERVER_H

#include "DoorLockHead.h"
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUFSIZE 512

//class
class Server {
private:
	int serverSocket;
	int clientSocket;
	struct sockaddr_in serverAddr;
	struct sockaddr_in clientAddr;
	char msgbuf[BUFSIZE + 1];

protected:
	int addrlen;
	
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
	
	//printError
	void showError(const char* msg);
};

#endif
