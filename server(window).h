#ifndef SERVER_H
#define SERVER_H

#include "DoorLockHead.h"

#define BUFSIZE 512

void showError(const char* msg);

void createServer(int PORT);

#endif
