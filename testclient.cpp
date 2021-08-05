#include <cstring>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
 
#define BUF_LEN 128
 
int main(int argc, char *argv[])
{
        int s, n;
        char* haddr;
        struct sockaddr_in server_addr;
        //struct sockaddr_in server_addr : 서버의 소켓주소 구조체
        char buf[BUF_LEN+1];
 
        if(argc != 2)
        {
                printf("usage : %s ip_Address\n", argv[0]);
                return 0;
        }
        haddr = argv[1];
 
        if((s = socket(PF_INET, SOCK_STREAM, 0)) < 0)
        {//소켓 생성과 동시에 소켓 생성 유효검사
                printf("can't create socket\n");
                return 0;
        }
 
        memset(&server_addr, 0, sizeof(server_addr));
        //서버의 소켓주소 구조체 server_addr을 NULL로 초기화
 
        server_addr.sin_family = AF_INET;
        //주소 체계를 AF_INET 로 선택
        server_addr.sin_addr.s_addr = inet_addr(argv[1]);
        //32비트의 IP주소로 변환
        server_addr.sin_port = htons(9000);
        //daytime 서비스 포트 번호
 
        if(connect(s, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        {//서버로 연결요청
                printf("can't connect.\n");
                return 0;
        }
 
        while((n = read(s, buf, BUF_LEN)) > 0)
        {//서버가 보내오는 daytime 데이터의 수신
                printf("%s", buf);
        }
 
        close(s);
        //사용이 완료된 소켓을 닫기
        return 0;
}
