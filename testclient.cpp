#include <stdio.h>
#include <stdlib.h>
#include <iostream>

//sensor control
#include <wiringPi.h>
#include <softPwm.h>

//server
#include <cstring>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

using namespace std;
 
#define BUF_LEN 512

int main(int argc, char *argv[])
{
        //server socket typedef
        int s;
        struct sockaddr_in server_addr;
        char buf[BUF_LEN];
        int bufsize = sizeof(buf);
 
        if(argc != 2) {
                printf("usage : %s ip_Address\n", argv[0]);
                return 0;
        }
        
        //create socket
        if((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
                printf("can't create socket\n");
                return 0;
        }
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = inet_addr(argv[1]);
        server_addr.sin_port = htons(9000);
 
        //connect to server
        if(connect(s, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
                printf("can't connect.\n");
                return 0;
        }
        
        while(1) {
                memset(buf, 0, bufsize);
                
                cout << "[CLIENT] : ";
                cin >> buf;
                send(s, buf, bufsize, 0);
                
                if(recv(s, buf, bufsize, 0) > 0) {
                        cout << "[SERVER] : "<< buf << endl;
                }
        }
        close(s);
        
        return 0;
}
