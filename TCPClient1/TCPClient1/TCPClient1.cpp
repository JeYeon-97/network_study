// TCP 클라이언트
//#pragma commnet(lib,"ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE 512

// 소켓 함수 오류
void err_quit(const char* msg) {
	exit(1); //강제종료 
}

// 소켓 함수 오류 출력
void err_display(const char* msg) {
	printf("오류");
}

// 데이터 수신 함수( 사용자 정의 함수)
int recvn(SOCKET s, char* buf, int len, int flags) {
	int received; // 수신 데이터 처리 변수

	char* ptr = buf; // 수신 버퍼의 시작 위치
	int left = len;  // 수신 버퍼의 아직 읽지 않은 값
	
	while (left > 0) {
		received = recv(s, ptr, left, flags);

		if (received == SOCKET_ERROR) {
			return SOCKET_ERROR;
		}
		else if (received == 0) {
			break;
		}

		left -= received;  // 읽은 남은 값
		ptr += received;   // 읽은 값
	}
	return (len - left); // 실제 읽은 값 리턴
}
int main() {
	int retval;

	// 1> 윈속 초기화 
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	// 2> 소켓 생성 
	SOCKET sock = socket(AF_INET6, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		err_quit("socket() 오류");
	}
	
	// 3> connect (서버 ip, 포트 번호)
	SOCKADDR_IN6 serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin6_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin6_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));

	if (retval == SOCKET_ERROR) {
		err_quit("connect() 오류");
	}

	// 4> 데이터 통신에 사용할 변수
	char buf[BUFSIZE + 1];
	int len;

	while (1) {
		printf("\n[보낼 데이터] : ");

		if (fgets(buf, BUFSIZE + 1, stdin) == NULL) {
			break;
		}
		len = strlen(buf); // 엔터키 없애기?
		if (buf[len - 1] == '\n') {
			buf[len - 1] = '\0';
		}
		if (strlen(buf) == 0) {
			break;
		}
		
		// 보내기
		retval = send(sock, buf, strlen(buf), 0); // 송신버퍼를 통해서 서버에 보내줌
		if (retval == SOCKET_ERROR) {
			err_display("send()오류");
			break;
		}

		printf("[TCP 클라이언트] %d 바이트를 보냈습니다 \n", retval);

		// 데이터 받기 
		retval == recvn(sock, buf,retval,0);
		if (retval == SOCKET_ERROR) {
			err_display("recvn()오류");
			break;
		}
		else if (retval == 0) {
			break;
		}

		buf[retval] = '\0';
		printf("[TCP 클라이언트] %d 바이트를 받았습니다.\n",retval);
		printf("[받은 데이터] %s \n", buf);


	}

	closesocket(sock);

	WSACleanup();

}