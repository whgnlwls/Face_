#ifndef SERVER_H
#define SERVER_H

#include "DoorLockHead.h"

#define BUFSIZE 512

void showError(const char* msg);

DWORD WINAPI clientThread(LPVOID lParam);

void createServer(int PORT);

#endif
