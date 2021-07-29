#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <iostream>
#include <WinSock2.h>
#pragma comment (lib , "ws2_32.lib")
#include <Windows.h>
#include "server.h"

#define BUFSIZE 512

void showError(const char* msg)
{
	std::cout << msg << std::endl;
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
		std::cout << "[CLIENT] [IP : %s, PORT : %d] : %s"
			<< inet_ntoa(clientAddr.sin_addr) << ntohs(clientAddr.sin_port) << msgbuf << std::endl;

		//server send message
		ret = send(clientSocket, msgbuf, ret, 0);
		if (ret == SOCKET_ERROR) {
			showError("server sending error");
			break;
		}
	}
	//close client socket
	closesocket(clientSocket);

	std::cout << "[SERVER] Client[IP : %s, PORT : %d] has exit"
		<< inet_ntoa(clientAddr.sin_addr) << ntohs(clientAddr.sin_port) << std::endl;

	return 0;
}

void createServer(const char* IPAdress, int PORT) {
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
		std::cout << "[SERVER] Accept client[IP : %s, PORT : %d]" 
			<< inet_ntoa(clientAddr.sin_addr) << ntohs(clientAddr.sin_port) << std::endl;

		//create client thread
		hthread = CreateThread(NULL, 0, clientThread, (LPVOID)clientSocket, 0, NULL);
		if (hthread == NULL) showError("create thread error");
		else CloseHandle(hthread);
	}
	//close server socket
	closesocket(serverSocket);

	WSACleanup();
}
