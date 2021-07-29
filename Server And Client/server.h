#ifndef SERVER_H
#define SERVER_H

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <WinSock2.h>
#pragma comment (lib , "ws2_32.lib")
#include <Windows.h>

#define BUFSIZE 512

void showError(const char* msg);

DWORD WINAPI clientThread(LPVOID lParam);

void createServer(const char* IPAdress, int PORT);

#endif
