#include "server.h"

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
					if (send(threadClientSocket, "adminstrator\r\n", msgbufsize, 0) <= 0) {
						cout << "[SERVER] : msg send error" << endl;
						break;
					}
					else {
						cout << "[SERVER] : send adminstrator to CLIENT["
						<< clientThreadAddr.sin_addr.s_addr
						<< ":" << clientThreadAddr.sin_port
						<< "]"<< endl;
						
						//send accounts list
						//read accounts info from file
						ifstream openFile(filePath.data());
						if(openFile.is_open()) {
							string accountData;
							while(getline(openFile, accountData)) {
								if (send(threadClientSocket, accountData + "\r\n", msgbufsize, 0) <= 0) {
									cout << "[SERVER] : msg send error" << endl;
									break;
								}
							}
							openFile.close();
							
							cout << "[SERVER] : send accounts info to CLIENT["
							<< clientThreadAddr.sin_addr.s_addr
							<< ":" << clientThreadAddr.sin_port
							<< "]"<< endl;
						}
					}
				}
				else {
					if (send(threadClientSocket, "general\r\n", msgbufsize, 0) <= 0) {
						cout << "[SERVER] : msg send error" << endl;
						break;
					}
					else {
						cout << "[SERVER] : send general to CLIENT["
						<< clientThreadAddr.sin_addr.s_addr
						<< ":" << clientThreadAddr.sin_port
						<< "]"<< endl;
					}
					//DB search account function

				}
			}
			else if (tokenVector[0] == "exit") {
				//server send message
				if (send(threadClientSocket, "exit\r\n", msgbufsize, 0) <= 0) {
					cout << "[SERVER] : msg send error" << endl;
					break;
				}
				else {
					cout << "[SERVER] : send exit to CLIENT["
					<< clientThreadAddr.sin_addr.s_addr
					<< ":" << clientThreadAddr.sin_port
					<< "]"<< endl;

					cout << "[SERVER] : CLIENT[" << clientThreadAddr.sin_addr.s_addr
					<< ":" << clientThreadAddr.sin_port << "] has exit from client thread" << endl;

					pthread_exit((void*)&clientThread);
				}
			}
			else if (tokenVector[0] == "admin") {
				if (tokenVector[1] == "regist") {
					//server send message
					if(send(threadClientSocket, "regist\r\n", msgbufsize, 0) <= 0) {
						cout << "[SERVER] : msg send error" << endl;
						break;
					}
					else {
						cout << "[SERVER] : send regist to CLIENT["
						<< clientThreadAddr.sin_addr.s_addr
						<< ":" << clientThreadAddr.sin_port
						<< "]"<< endl;
						
						string filePath = "/home/pi/testsrc/userAccount.txt";
						
						//add accounts info to file
						ofstream writeFile(filePath.data(), ios::app);
						if(writeFile.is_open()) {
							writeFile << tokenVector[2] << "$" << tokenVector[3] << "$" << endl;
							writeFile.close();
						}
						
						//read accounts info from file
						ifstream openFile(filePath.data());
						if(openFile.is_open()) {
							string accountData;
							while(getline(openFile, accountData)) {
								cout << accountData << endl;
							}
							openFile.close();
						}
						
						//openCV camera control function
					}
				}
				else if (tokenVector[1] == "deregist") {
					//server send message
					if(send(threadClientSocket, "deregist\r\n", msgbufsize, 0) <= 0) {
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
					if(send(threadClientSocket, "command error\r\n", msgbufsize, 0) <= 0) {
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
					if (send(threadClientSocket, "open\r\n", msgbufsize, 0) <= 0) {
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
					if (send(threadClientSocket, "close\r\n", msgbufsize, 0) <= 0) {
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
					if (send(threadClientSocket, "logout\r\n", msgbufsize, 0) <= 0) {
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
					if(send(threadClientSocket, "command error\r\n", msgbufsize, 0) <= 0) {
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
				if(send(threadClientSocket, "command error\r\n", msgbufsize, 0) <= 0) {
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
