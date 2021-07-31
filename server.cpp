#include "server.h"

#define BUFSIZE 512

//initialize
//create socket
Server::Server(int PORT) {
	if((Server::serverSocket = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		Server::showError("create server socket error");
		exit(-1);
	}
}

//close socket
Server::~Server() {
	close(Server::serverSocket);
}

//bind
void Server::bindSocket() {
	
}

//listen
void Server::listenSocket() {
	
}

//accept
void Server::acceptSocket() {
	
}

//print error
void Server::showError(const char* msg) {
	cout << msg << endl;
}
