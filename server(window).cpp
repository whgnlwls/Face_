#include "server.h"

#define BUFSIZE 512

void showError(const char* msg)
{
	cout << msg << endl;
}

DWORD WINAPI clientThread(LPVOID lParam) {
	//create client socket
	SOCKET clientSocket = (SOCKET)lParam;
	char msgbuf[BUFSIZE + 1];
	SOCKADDR_IN clientAddr;
	int addrlen;
	int ret;

	addrlen = sizeof(clientAddr);
	getpeername(clientSocket, (SOCKADDR*)&clientAddr, &addrlen);

	while (true) {
		//server recive message
		ret = recv(clientSocket, msgbuf, BUFSIZE, 0);
		if (ret == SOCKET_ERROR) {
			showError("server reciving error");
			break;
		}
		else if (ret == 0) break;

		msgbuf[ret] = '\0';
		cout << "[CLIENT] [IP : %s, PORT : %d] : %s"
			<< inet_ntoa(clientAddr.sin_addr) << ntohs(clientAddr.sin_port) << msgbuf << endl;

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
				ret = send(clientSocket, "adminstrator", ret, 0);
				if (ret == SOCKET_ERROR) {
					showError("server sending error");
					break;
				}
			}
			//or?
			//coding anything
		}
		else if (tokenVector[0] == "open") {

		}
		else if (tokenVector[0] == "close") {

		}
	}
	//close client socket
	closesocket(clientSocket);

	cout << "[SERVER] Client[IP : %s, PORT : %d] has exit"
		<< inet_ntoa(clientAddr.sin_addr) << ntohs(clientAddr.sin_port) << endl;

	return 0;
}

void createServer(int PORT) {
	//create server socket
	int recval;
	SOCKET serverSocket;
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) showError("create winsock error");

	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == INVALID_SOCKET) showError("create server socket error");

	SOCKADDR_IN serverAddr;
	ZeroMemory(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = PORT;
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	//bind server socket
	recval = bind(serverSocket, (SOCKADDR*)&serverAddr, sizeof(serverSocket));
	if (recval == INVALID_SOCKET) showError("server binding error");

	//listen server socket
	recval = listen(serverSocket, SOMAXCONN);
	if (recval == INVALID_SOCKET) showError("server listening error");

	//create client socket
	SOCKET clientSocket;
	SOCKADDR_IN clientAddr;
	int addrlen;

	HANDLE hthread;

	//accept client
	while (true) {
		addrlen = sizeof(clientAddr);
		clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddr, &addrlen);
		if (clientSocket == INVALID_SOCKET) {
			showError("server accepting error");
			continue;
		}
		cout << "[SERVER] Accept client[IP : %s, PORT : %d]" 
			<< inet_ntoa(clientAddr.sin_addr) << ntohs(clientAddr.sin_port) << endl;

		//create client thread
		hthread = CreateThread(NULL, 0, clientThread, (LPVOID)clientSocket, 0, NULL);
		if (hthread == NULL) showError("create thread error");
		else CloseHandle(hthread);
	}
	//close server socket
	closesocket(serverSocket);

	WSACleanup();
}
