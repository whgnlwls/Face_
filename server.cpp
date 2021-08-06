#include "server.h"

//initialize
//create socket
Server::Server(int PORT) {
	if((serverSocket = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		showError("create server socket error");
	}
	else {
		cout << "create server socket success" << endl;
	
		memset(&serverAddr, 0, sizeof(serverAddr));
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(PORT);
		serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
}

//close socket
Server::~Server() {
	pthread_exit((void*)clientThread);
	
	close(serverSocket);
}

//bind
void Server::bindSocket() {
	if(bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
		showError("bind server socket error");
	}
	else {
		cout << "bind server socket success" << endl;
	}
}

//listen
void Server::listenSocket() {
	if(listen(serverSocket, 1) < 0) {
		showError("server listen error");
	}
	else {
		cout << "server listen success" << endl;
	}
}

//accept
void Server::acceptSocket() {
	//create client thread
	pthread_t cthread;
	int clientAddrlen = sizeof(clientAddr);
	while(1) {
		clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, (socklen_t*)&clientAddrlen);
		if(clientSocket < 0) {
			cout << "accept client error" << endl;
		}
		else {
			cout << "accept client" << "[" << clientAddr.sin_addr.s_addr << ":" << clientAddr.sin_port << "] success" << endl;
			pthread_create(&cthread, NULL, clientThread, (void*)&clientSocket);
		}
	}
}

//client thread
void* Server::clientThread(void* clientSock) {
	int threadClientSocket = *(int*)clientSock;
	char msgbuf[BUFSIZE];
	int msgbufsize = sizeof(msgbuf);
	cout << "create thread success" << endl;
	
	while(1) {
		memset(msgbuf, 0, msgbufsize);
		
		if(read(threadClientSocket, msgbuf, msgbufsize) <= 0) {
		cout << "msg recive error" << endl;
		break;
		}
		else {
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
					if (write(threadClientSocket, "adminstrator", msgbufsize) <= 0) {
						cout << "msg send error" << endl;
						break;
					}
					else {
						cout << "[SERVER] : send adminstrator to client" << endl;
					}
				}
			}
			else if (tokenVector[0] == "open") {
				cout << "now open" << endl;
			}
			else if (tokenVector[0] == "close") {
				cout << "now close" << endl;
			}
		}
	}
	close(threadClientSocket);
	
	return 0;
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
	cout << "create server success" << endl;
	server.acceptSocket();
}
