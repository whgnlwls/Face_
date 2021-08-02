#include "server.h"

//client thread
void* Server::clientThread(void* clientSock) {
	int threadClientSocket = *(int*)clientSock;
	
	while(true) {
		if(recv(threadClientSocket, msgbuf, BUFSIZE, 0) < 0) {
		showError("msg recive error");
		}
		cout << "[CLIENT] : " << msgbuf << endl;
	
		//create token
		vector<string> tokenVector;
		stringstream msgStream(msgbuf);
		string tokenizer;
	
		while (getline(msgStream, tokenizer, '$')) {
			tokenVector.push_back(tokenizer);
		}

		//token works
		if (tokenVector[0] == "login") {
			//admin?
			if (tokenVector[1] == "admin" && tokenVector[2] == "admin") {
				//server send message
				if (send(threadClientSocket, "adminstrator", threadClientSocket, 0) < 0) {
					showError("msg send error");
					break;
				}
				cout << "[SERVER] : send adminstrator to client" << endl;
			}
			//or?
			//coding anything
		
		}
		else if (tokenVector[0] == "open") {

		}
		else if (tokenVector[0] == "close") {
	
		}
	}
	return 0;
}

//initialize
//create socket
Server::Server(int PORT) {
	if((serverSocket = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		showError("create server socket error");
	}
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
}

//close socket
Server::~Server() {
	close(serverSocket);
}

//bind
void Server::bindSocket() {
	if(bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
		showError("bind server socket error");
	}
}

//listen
void Server::listenSocket() {
	if(listen(serverSocket, 1) < 0) {
		showError("server listen error");
	}
}

//accept
void Server::acceptSocket() {
	//create client thread
	pthread_t cthread;
	while(true) {
		clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, (socklen_t*)sizeof(clientAddr));
		if(clientSocket < 0) {
			showError("accept client error");
		}
		pthread_create(&cthread, NULL, clientThread, (void*)&clientSocket);
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
