#include "server.h"

//client thread
void* Server::clientThread(void* clientSock) {
	int threadClientSocket = *(int*)clientSock;
	if(recv(threadClientSocket, msgbuf, BUFSIZE, 0) < 0) {
		showError("msg recive error");
	}
	cout << "[CLIENT] : " << msgbuf << endl;
	
	//create token
	
}

//initialize
//create socket
Server::Server(int PORT) {
	if((Server::serverSocket = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		Server::showError("create server socket error");
	}
	Server::serverAddr.sin_family = AF_INET;
	Server::serverAddr.sin_port = htons(PORT);
	Server::serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
}

//close socket
Server::~Server() {
	close(Server::serverSocket);
}

//bind
void Server::bindSocket() {
	if(bind(Server::serverSocket, (struct sockaddr*)&Server::serverAddr, sizeof(Server::serverAddr)) < 0) {
		Server::showError("bind server socket error");
	}
}

//listen
void Server::listenSocket() {
	if(listen(Server::serverSocket, 1) < 0) {
		showError("server listen error");
	}
}

//accept
void Server::acceptSocket() {
	//create client thread
	pthread_t cthread;
	while(true) {
		Server::clientSocket = accept(Server::serverSocket, (struct sockaddr*)&Server::clientAddr, (socklen_t*)sizeof(Server::clientAddr));
		if(Server::clientSocket < 0) {
			Server::showError("accept client error");
		}
		pthread_create(&cthread, NULL, clientThread, (void*)&Server::clientSocket);
	}
}

//print error
void Server::showError(const char* msg) {
	cout << msg << endl;
	exit(1);
}

//create server
void Server::createServer(int PORT) {
	Server server = Server(PORT);
	server.bindSocket();
	server.listenSocket();
	server.acceptSocket();
}
