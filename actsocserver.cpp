#include "actsocserver.h"

//create socket
Server::Server(int PORT) {
	if((serverSocket = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		showError("[SERVER] : create server socket error");
	}
	else {
		cout << "[SERVER] : create server socket success" << endl;
		
		//set server socket typedef
		memset(&serverAddr, 0, sizeof(serverAddr));
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(PORT);
		serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
}

//close socket
Server::~Server() {
	close(clientSocket);
	close(serverSocket);
}

//bind
void Server::bindSocket() {
	if(bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
		showError("[SERVER] : bind server socket error");
	}
	else {
		cout << "[SERVER] : bind server socket success" << endl;
	}
}

//listen
void Server::listenSocket() {
	if(listen(serverSocket, 1) < 0) {
		showError("[SERVER] : server listen error");
	}
	else {
		cout << "[SERVER] : server listen success" << endl;
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
			cout << "[SERVER] : accept client error" << endl;
		}
		else {
			cout << "[SERVER] : CLIENT[" << clientAddr.sin_addr.s_addr << ":" 
			<< clientAddr.sin_port << "] accept success" << endl;
			
			//create login thread
			pthread_create(&cthread, NULL, clientThread, (void*)&clientSocket);
		}
	}
}

//client thread
void* Server::clientThread(void* clientSock) {
	//thread init
	int threadClientSocket = *(int*)clientSock;
	char msgbuf[BUFSIZE];
	int msgbufsize = sizeof(msgbuf);
	
	//set client socket typedef
	sockaddr_in clientThreadAddr;
	socklen_t clientThreadAddrlen = sizeof(clientThreadAddr);
	getpeername(threadClientSocket, (sockaddr*)&clientThreadAddr, &clientThreadAddrlen);
	
	cout << "[SERVER] : create CLIENT[" << clientThreadAddr.sin_addr.s_addr 
	<< ":" << clientThreadAddr.sin_port << "] client thread success" << endl;
	
	//thread work
	while(1) {
		//set buffer
		memset(msgbuf, 0, msgbufsize);
		
		if(recv(threadClientSocket, msgbuf, msgbufsize, 0) <= 0) {
			cout << "[SERVER] : msg receive error" << endl;
			break;
		}
		else {
			cout << "[SERVER] : CLIENT[" << clientThreadAddr.sin_addr.s_addr 
			<< ":" << clientThreadAddr.sin_port << "] : " << msgbuf << endl;
			
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
					if (send(threadClientSocket, "adminstrator", msgbufsize, 0) <= 0) {
						cout << "[SERVER] : msg send error" << endl;
						break;
					}
					else {
						cout << "[SERVER] : send adminstrator to CLIENT[" 
						<< clientThreadAddr.sin_addr.s_addr 
						<< ":" << clientThreadAddr.sin_port 
						<< "]"<< endl;
					}
				}
				else {
					//DB search account function
				}				
			}
			else if (tokenVector[0] == "exit") {
				//server send message
				if (send(threadClientSocket, "exit", msgbufsize, 0) <= 0) {
					cout << "[SERVER] : msg send error" << endl;
					break;
				}
				else {
					cout << "[SERVER] : send exit to CLIENT[" 
					<< clientThreadAddr.sin_addr.s_addr 
					<< ":" << clientThreadAddr.sin_port 
					<< "]"<< endl;
					
					cout << "[SERVER] : CLIENT[" << clientThreadAddr.sin_addr.s_addr 
					<< ":" << clientThreadAddr.sin_port << "] has exit from login thread" << endl;
					
					pthread_exit((void*)&clientThread);
				}
			}
			else if (tokenVector[0] == "admin") {
				if (tokenVector[1] == "regist") {
					//server send message
					if(send(threadClientSocket, "regist", msgbufsize, 0) <= 0) {
						cout << "[SERVER] : msg send error" << endl;
						break;
					}
					else {
						cout << "[SERVER] : send regist to CLIENT[" 
						<< clientThreadAddr.sin_addr.s_addr 
						<< ":" << clientThreadAddr.sin_port 
						<< "]"<< endl;
						
						//openCV camera control function
					}
				}
				else if (tokenVector[1] == "deregist") {
					//server send message
					if(send(threadClientSocket, "deregist", msgbufsize, 0) <= 0) {
						cout << "[SERVER] : msg send error" << endl;
						break;
					}
					else {
						cout << "[SERVER] : send deregist to CLIENT[" 
						<< clientThreadAddr.sin_addr.s_addr 
						<< ":" << clientThreadAddr.sin_port 
						<< "]"<< endl;
					
						//DB remove function
					}
				}
				else {
					//server send message
					if(send(threadClientSocket, "command error", msgbufsize, 0) <= 0) {
						cout << "[SERVER] : msg send error" << endl;
						break;
					}
					else {
						cout << "[SERVER] : send command error to CLIENT[" 
						<< clientThreadAddr.sin_addr.s_addr 
						<< ":" << clientThreadAddr.sin_port 
						<< "]"<< endl;
					}
				}
			}
			else if (tokenVector[0] == "general") {
				if (tokenVector[1] == "open") {
				//server send message
					if (send(threadClientSocket, "open", msgbufsize, 0) <= 0) {
						cout << "[SERVER] : msg send error" << endl;
						break;
					}
					else {
						cout << "[SERVER] : send open to CLIENT[" 
						<< clientThreadAddr.sin_addr.s_addr 
						<< ":" << clientThreadAddr.sin_port 
						<< "]"<< endl;
					}
				}
				else if (tokenVector[1] == "close") {
					//server send message
					if (send(threadClientSocket, "close", msgbufsize, 0) <= 0) {
						cout << "[SERVER] : msg send error" << endl;
						break;
					}
					else {
						cout << "[SERVER] : send close to CLIENT[" 
						<< clientThreadAddr.sin_addr.s_addr 
						<< ":" << clientThreadAddr.sin_port 
						<< "]"<< endl;
					}
				}
				else if (tokenVector[1] == "logout") {
					//server send message
					if (send(threadClientSocket, "logout", msgbufsize, 0) <= 0) {
						cout << "[SERVER] : msg send error" << endl;
						break;
					}
					else {
						cout << "[SERVER] : send logout to CLIENT[" 
						<< clientThreadAddr.sin_addr.s_addr 
						<< ":" << clientThreadAddr.sin_port 
						<< "]"<< endl;
					}
				}
				else {
					//server send message
					if(send(threadClientSocket, "command error", msgbufsize, 0) <= 0) {
						cout << "[SERVER] : msg send error" << endl;
						break;
					}
					else {
						cout << "[SERVER] : send command error to CLIENT[" 
						<< clientThreadAddr.sin_addr.s_addr 
						<< ":" << clientThreadAddr.sin_port 
						<< "]"<< endl;
					}
				}
			}
			else {
				//server send message
				if(send(threadClientSocket, "command error", msgbufsize, 0) <= 0) {
					cout << "[SERVER] : msg send error" << endl;
					break;
				}
				else {
					cout << "[SERVER] : send command error to CLIENT[" 
					<< clientThreadAddr.sin_addr.s_addr 
					<< ":" << clientThreadAddr.sin_port 
					<< "]"<< endl;
				}
			}
		}
	}
	
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
	cout << "[SERVER] : create server success" << endl;
	server.acceptSocket();
}
