#ifndef SERVER_H
#define SERVER_H

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <WinSock2.h>
#pragma comment (lib , "ws2_32.lib")
#include <Windows.h>

#define BUFSIZE 512

void showError(const char* msg);

void createServer(int PORT);

#endif
