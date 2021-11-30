#include "server.h"

//create socket
Server::Server(int pinBuzzer, int F_pinUltraSonicTrig, int F_pinUltraSonicEcho, int B_pinUltraSonicTrig, int B_pinUltraSonicEcho, int pinMotor, int PORT)
 : SensorControl(pinBuzzer, F_pinUltraSonicTrig, F_pinUltraSonicEcho, B_pinUltraSonicTrig, B_pinUltraSonicEcho, pinMotor) {
	if((serverSocket = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		showError("[SERVER] : create server socket error");
	}
	else {
		this->pythonSwitch = 0;
		this->isWindow = 0;
		this->screen = imread(this->path_image_wait);
		
		pthread_t dthread, pythread;
		
		pthread_create(&pythread, NULL, pythonProcessThread, (void*)this);
		pthread_create(&dthread, NULL, doorlockThread, (void*)this);
		
		//set server socket typedef
		memset(&serverAddr, 0, sizeof(serverAddr));
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(PORT);
		serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		
		createServer();
	}
}

//close socket
Server::~Server() {
	close(Server::clientSocket);
	close(Server::serverSocket);
}

//doorlock thread
void* Server::doorlockThread(void* pClass) {
	Server* sClass;
	
	do {
		sClass = (Server*)pClass;
		
		//ultra sonic sensor work
		if(sClass->isCameraUse != 1) {
			sClass->F_USgetDist();
			sClass->B_USgetDist();
			delay(10);
			cout << endl;
			
			if(sClass->F_getDist() < F_US_STD_DIST && sClass->pythonSwitch == 0) {
				//openCV face detect
				sClass->pythonSwitch = 3;
				while(!sClass->pythonSwitch == 0);
				
				//get confidence
				string confidence;
				ifstream confidenceFile(sClass->path_openCV_Confidence.data());
				if(confidenceFile.is_open()) {
					getline(confidenceFile, confidence);
					confidenceFile.close();
				}
				
				//if face know door open
				int count = 0;
				if(stoi(confidence)) {
					sClass->MTsetOpen();
					do {
						sClass->B_USgetDist();
						delay(5);
						count++;
						if(count > 20) sClass->BZsetBuzzer();
					} while(sClass->B_getDist() > B_US_STD_DIST);
					delay(100);
					sClass->MTsetClose();
					
					//set 0 confidence
					ofstream writeFile(sClass->path_openCV_Confidence.data());
					if(writeFile.is_open()) {
						writeFile << 0;
						
						writeFile.close();
					}
				}
				else {
					sClass->pythonSwitch = 4;
					sClass->BZsetBuzzer();
				}
			}
		}
	} while(1);
	
	return 0;
}

//bind
void Server::bindSocket() {
	if(bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
		showError("[SERVER] : bind server socket error");
	}
}

//listen
void Server::listenSocket() {
	if(listen(serverSocket, 1) < 0) {
		showError("[SERVER] : server listen error");
	}
}

//accept      last error : accept is zombie
void Server::acceptSocket() {
	//create client thread
	pthread_t cthread;

	while(1) {
		clientAddrlen = sizeof(clientAddr);
		clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, (socklen_t*)&clientAddrlen);
		
		if(clientSocket < 0) {
			cout << "[SERVER] : accept client error" << endl;
		}
		else {
			CLCR clientData;
			clientData.CLCR_Socket = clientSocket;
			clientData.CLCR_pClass = this;
			
			pthread_create(&cthread, NULL, clientThread, (void*)&clientData);
		}
	}
}

//client thread
void* Server::clientThread(void* clientData) {
	//thread init
	CLCR client = *(CLCR*)clientData;
	
	int threadClientSocket = client.CLCR_Socket;
	char msgbuf[BUFSIZE];
	int msgbufsize = sizeof(msgbuf);

	//set client socket typedef
	sockaddr_in clientThreadAddr;
	socklen_t clientThreadAddrlen = sizeof(clientThreadAddr);
	getpeername(threadClientSocket, (sockaddr*)&clientThreadAddr, &clientThreadAddrlen);

	//thread work
	//set buffer
	memset(msgbuf, 0, msgbufsize);
	
	if(recv(threadClientSocket, msgbuf, msgbufsize, 0) <= 0) {}
	else {
		cout << "\n[CLIENT(" << clientThreadAddr.sin_addr.s_addr
		<< ":" << clientThreadAddr.sin_port << ")] : " << msgbuf;

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
				}
				else {
					cout << "[SERVER] : send adminstrator to CLIENT["
					<< clientThreadAddr.sin_addr.s_addr
					<< ":" << clientThreadAddr.sin_port
					<< "]"<< endl;
				}
			}
			else {
				//check vaild account
				vector<string> accountTokenVector;
				string accountData;
				int ID_checked;
				int PWD_checked;
					
				ifstream openFile(client.CLCR_pClass->path_account.data());
				if(openFile.is_open()) {
					while(getline(openFile, accountData, '$')) {
						ID_checked = 0;
						PWD_checked = 0;
							
						istringstream account(accountData);
						while(getline(account, accountData, ',')) {
							if(tokenVector[1] == accountData) ID_checked = 1;
							if(tokenVector[2] == accountData) PWD_checked = 1;
						}
							
						if(ID_checked == 1 && PWD_checked == 1) break;
						else if (ID_checked == 1 && PWD_checked != 1) break;
					}
					openFile.close();
				}
					
				if(ID_checked == 1 && PWD_checked == 1) {
					if (send(threadClientSocket, "general\r\n", msgbufsize, 0) <= 0) {
						cout << "[SERVER] : msg send error" << endl;
					}
					else {
						cout << "[SERVER] : send general to CLIENT["
						<< clientThreadAddr.sin_addr.s_addr
						<< ":" << clientThreadAddr.sin_port
						<< "]"<< endl;
					}
				}
				else if(ID_checked == 1 && PWD_checked != 1) {
					if (send(threadClientSocket, "invalid_pwd\r\n", msgbufsize, 0) <= 0) {
						cout << "[SERVER] : msg send error" << endl;
					}
					else {
						cout << "[SERVER] : send invalid_pwd to CLIENT["
						<< clientThreadAddr.sin_addr.s_addr
						<< ":" << clientThreadAddr.sin_port
						<< "]"<< endl;
					}
				}
				else {
					if (send(threadClientSocket, "invalid_acc\r\n", msgbufsize, 0) <= 0) {
						cout << "[SERVER] : msg send error" << endl;
					}
					else {
						cout << "[SERVER] : send invalid_acc to CLIENT["
						<< clientThreadAddr.sin_addr.s_addr
						<< ":" << clientThreadAddr.sin_port
						<< "]"<< endl;
					}
				}
			}
		}
		else if (tokenVector[0] == "id_list") {
			//send accounts list to client
			vector<string> accountTokenVector;

			ifstream openFile(client.CLCR_pClass->path_account.data());
			if(openFile.is_open()) {
				string accountData;
				string accountDataID;
					
				//get account data from DB
				while(getline(openFile, accountData, '$')) {
					accountTokenVector.push_back(accountData);
				}
				openFile.close();
					
				for(unsigned int i = 0; i < accountTokenVector.size(); i++) {
					istringstream accountID(accountTokenVector[i]);
					getline(accountID, accountTokenVector[i], ',');
				}
					
				//make send message
				for(unsigned int i = 0; i < accountTokenVector.size(); i++) {
					accountDataID += accountTokenVector[i];
					accountDataID += "$";
				}
				accountDataID += "\r\n";
					
				//server send message
				if (send(threadClientSocket, accountDataID.c_str(), msgbufsize, 0) <= 0) {
					cout << "[SERVER] : msg send error" << endl;
				}
				else {
					if(accountTokenVector.size() != 0) {
						cout << "[SERVER] : send accounts info list to CLIENT["
						<< clientThreadAddr.sin_addr.s_addr
						<< ":" << clientThreadAddr.sin_port
						<< "]\n[ACCOUNT LIST] : " << accountDataID;
					}
					else {
						cout << "[SERVER] : send accounts info list to CLIENT["
						<< clientThreadAddr.sin_addr.s_addr
						<< ":" << clientThreadAddr.sin_port
						<< "] : " << "empty list" << endl;
					}
				}
			}
		}
		else if (tokenVector[0] == "admin") {
			if (tokenVector[1] == "regist") {
				//is same ID on DB
				vector<string> accountTokenVector;
				string accountData;
				string accountDataID;
				int isSameID = 0;
					
				ifstream openFile(client.CLCR_pClass->path_account.data());
				if(openFile.is_open()) {
					while(getline(openFile, accountData, '$')) {
						accountTokenVector.push_back(accountData);
					}
					openFile.close();
				}
				for(unsigned int i = 0; i < accountTokenVector.size(); i++) {
					istringstream accountID(accountTokenVector[i]);
					getline(accountID, accountDataID, ',');
					if(accountDataID == tokenVector[2]) {
						isSameID = 1;
						if(send(threadClientSocket, "sameID\r\n", msgbufsize, 0) <= 0) {
							cout << "[SERVER] : msg send error" << endl;
						}
						else {
							cout << "[SERVER] : send sameID to CLIENT["
							<< clientThreadAddr.sin_addr.s_addr
							<< ":" << clientThreadAddr.sin_port
							<< "]"<< endl;
						}
					}
				}
				
				if(isSameID == 0) {
					//server send message
					if(send(threadClientSocket, "regist\r\n", msgbufsize, 0) <= 0) {
						cout << "[SERVER] : msg send error" << endl;
					}
					else {
						cout << "[SERVER] : send regist to CLIENT["
						<< clientThreadAddr.sin_addr.s_addr
						<< ":" << clientThreadAddr.sin_port
						<< "]"<< endl;
						
						//add accounts info to DB
						ofstream writeFile(client.CLCR_pClass->path_account.data(), ios::app);
						if(writeFile.is_open()) {
							writeFile << tokenVector[2] << "," << tokenVector[3] << "$";
							writeFile.close();
						}
						cout << "[SERVER] : regist account success ["
						<< tokenVector[2] << "," << tokenVector[3] << "]" << endl;
						
						//openCV camera control
						ofstream IDsender(client.CLCR_pClass->path_openCV_ID.data());
						if(IDsender.is_open()) {
							IDsender << tokenVector[2];
							IDsender.close();
						}
						
						while(!client.CLCR_pClass->isCameraUse == 0);
						client.CLCR_pClass->isCameraUse = 1;
						
						while(!client.CLCR_pClass->pythonSwitch == 0);
						client.CLCR_pClass->pythonSwitch = 1;
						
						while(!client.CLCR_pClass->pythonSwitch == 0);
						client.CLCR_pClass->pythonSwitch = 2;
						
						client.CLCR_pClass->isCameraUse = 0;
					}
				}
			}
			else if (tokenVector[1] == "deregist") {
				//server send message
				if(send(threadClientSocket, "deregist\r\n", msgbufsize, 0) <= 0) {
					cout << "[SERVER] : msg send error" << endl;
				}
				else {
					cout << "[SERVER] : send deregist to CLIENT["
					<< clientThreadAddr.sin_addr.s_addr
					<< ":" << clientThreadAddr.sin_port
					<< "]"<< endl;

					//search deregist accounts info from DB
					vector<string> beforeAccountTokenVector;
					vector<string> afterAccountTokenVector;
					string accountData;
					string accountDataID;
						
					ifstream openFile(client.CLCR_pClass->path_account.data());
					if(openFile.is_open()) {
						while(getline(openFile, accountData, '$')) {
							beforeAccountTokenVector.push_back(accountData);
						}
						openFile.close();
					
						for(unsigned int i = 0; i < beforeAccountTokenVector.size(); i++) {
							istringstream accountID(beforeAccountTokenVector[i]);
							getline(accountID, accountDataID, ',');
								
							if(accountDataID == tokenVector[2]) {
								accountData = beforeAccountTokenVector[i];
								
								cout << "[SERVER] : deregist account success ["
								<< accountData << "]" << endl;
							}
						}
						
						//remove picture
						string removePic;
						removePic += "rm -r /home/pi/testsrc/facede/dataset/User.";
						removePic += tokenVector[2].c_str();
						removePic +=".*";
						system(removePic.c_str());
						
						while(!client.CLCR_pClass->pythonSwitch == 0);
						client.CLCR_pClass->pythonSwitch = 2;
					}

					//set new account array
					for(unsigned int i = 0; i < beforeAccountTokenVector.size(); i++) {
						if(beforeAccountTokenVector[i] != accountData) {
							afterAccountTokenVector.push_back(beforeAccountTokenVector[i]);
						}
					}
						
					//refresh accounts info to DB
					ofstream writeFile(client.CLCR_pClass->path_account.data());
					if(writeFile.is_open()) {
						for(unsigned int i = 0; i < afterAccountTokenVector.size(); i++) {
							writeFile << afterAccountTokenVector[i] << "$";
						}
						writeFile.close();
					}
				}
			}
			else {
				//server send message
				if(send(threadClientSocket, "command error\r\n", msgbufsize, 0) <= 0) {
					cout << "[SERVER] : msg send error" << endl;
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
				}
				else {
					cout << "[SERVER] : send open to CLIENT["
					<< clientThreadAddr.sin_addr.s_addr
					<< ":" << clientThreadAddr.sin_port
					<< "]"<< endl;
						
					//unlock
					client.CLCR_pClass->MTsetOpen();
				}
			}
			else if (tokenVector[1] == "close") {
				//server send message
				if (send(threadClientSocket, "close\r\n", msgbufsize, 0) <= 0) {
					cout << "[SERVER] : msg send error" << endl;
				}
				else {
					cout << "[SERVER] : send close to CLIENT["
					<< clientThreadAddr.sin_addr.s_addr
					<< ":" << clientThreadAddr.sin_port
					<< "]"<< endl;
					
					//lock
					client.CLCR_pClass->MTsetClose();
				}
			}
			else if (tokenVector[1] == "logout") {
				//server send message
				if (send(threadClientSocket, "logout\r\n", msgbufsize, 0) <= 0) {
					cout << "[SERVER] : msg send error" << endl;
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
			}
			else {
				cout << "[SERVER] : send command error to CLIENT["
				<< clientThreadAddr.sin_addr.s_addr
				<< ":" << clientThreadAddr.sin_port
				<< "]"<< endl;
			}
		}
	}

	return 0;
}

void* Server::pythonProcessThread(void* pClass) {
	Py_Initialize();
	Server* sClass;
	
	do {
		sClass = (Server*)pClass;
		
		if((sClass->pythonSwitch == 0 && sClass->isWindow == 0) || sClass->pythonSwitch == 4) {
			if(sClass->pythonSwitch == 4) {
				destroyWindow("wait");
				sClass->isWindow = 0;
				
				sClass->screen = imread(sClass->path_image_fail);
				namedWindow("fail");
				imshow("fail", sClass->screen);
				setWindowProperty("fail",WND_PROP_FULLSCREEN, WINDOW_FULLSCREEN);
				moveWindow("fail", 0, 0);
				sClass->isWindow = 1;
				waitKey(5000);
				
				destroyWindow("fail");
				sClass->pythonSwitch = 0;
				sClass->isWindow = 0;
				sClass->screen = imread(sClass->path_image_wait);
			}
			else if(sClass->pythonSwitch == 0) {
				namedWindow("wait");
				imshow("wait", sClass->screen);
				setWindowProperty("wait",WND_PROP_FULLSCREEN, WINDOW_FULLSCREEN);
				moveWindow("wait", 0, 0);
				sClass->isWindow = 1;
				waitKey(1000);
			}	
		}
		else if(sClass->pythonSwitch == 1) {
			destroyWindow("wait");
			sClass->isWindow = 0;
			PyObject *obj = Py_BuildValue("s", sClass->path_openCV_imageCapture.c_str());
			FILE *file = _Py_fopen_obj(obj, "r+");
							
			if(file != NULL) {
				PyRun_SimpleFile(file, sClass->path_openCV_imageCapture.c_str());
			}
			sClass->pythonSwitch = 0;
		}
		else if(sClass->pythonSwitch == 2) {
			PyObject *obj = Py_BuildValue("s", sClass->path_openCV_imageModeling.c_str());
			FILE *file = _Py_fopen_obj(obj, "r+");
							
			if(file != NULL) {
				PyRun_SimpleFile(file, sClass->path_openCV_imageModeling.c_str());
			}
			sClass->pythonSwitch = 0;
		}
		else if(sClass->pythonSwitch == 3) {
			destroyWindow("wait");
			sClass->isWindow = 0;
			PyObject *obj = Py_BuildValue("s", sClass->path_openCV_imageDetect.c_str());
			FILE *file = _Py_fopen_obj(obj, "r+");
							
			if(file != NULL) {
				PyRun_SimpleFile(file, sClass->path_openCV_imageDetect.c_str());
			}
			sClass->pythonSwitch = 0;
		}
	} while(1);
	
	Py_Finalize();
	
	return 0;
}

//print error
void Server::showError(const char* msg) {
	cout << msg << endl;
	exit(1);
}

//create server
void Server::createServer() {
	bindSocket();
	listenSocket();
	cout << "[SERVER] : create server success" << endl;
	acceptSocket();
}
