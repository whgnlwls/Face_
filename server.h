#ifndef SERVER_H
#define SERVER_H

#include "sensorControl.h"

#define BUFSIZE 512

//class
class Server : public SensorControl {
private:
	int serverSocket;
	int clientSocket;
	
	struct sockaddr_in serverAddr;
	struct sockaddr_in clientAddr;
	int clientAddrlen;
	int isWindow;
	
	//Image path
	string path_image_wait = "/home/pi/testsrc/wait.png";
	string path_image_fail = "/home/pi/testsrc/fail.png";
	
	//Image Mat
	Mat screen;
	
	//Value path
	string path_account = "/home/pi/testsrc/userAccount.txt";;
	string path_openCV_ID = "/home/pi/testsrc/openCV_ID.txt";
	string path_openCV_Confidence = "/home/pi/testsrc/openCV_confidence.txt";
	
	//python process path
	string path_openCV_imageCapture = "/home/pi/testsrc/facede/NewModeling.py";
	string path_openCV_imageModeling = "/home/pi/testsrc/facede/2.py";
	string path_openCV_imageDetect = "/home/pi/testsrc/facede/3.py";

protected:
	
public:
	//initialize
	Server() {};
	Server(int pinBuzzer, int F_pinUltraSonicTrig, int F_pinUltraSonicEcho, int B_pinUltraSonicTrig, int B_pinUltraSonicEcho, int pinMotor, int PORT);
	~Server();
	int pythonSwitch;
	
	//bind
	void bindSocket();
	
	//listen
	void listenSocket();
	
	//accept
	void acceptSocket();
	
	//thread
	static void* clientThread(void* client);
	static void* doorlockThread(void* pClass);
	static void* pythonProcessThread(void* pClass);

	//print error
	void showError(const char* msg);
	
	//create server
	void createServer();
};

//type define
typedef struct ClientControl {
	Server* CLCR_pClass;
	int CLCR_Socket;
}CLCR;

#endif
