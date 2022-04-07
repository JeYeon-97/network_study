//TCPClient : IPV4

// 데이터 전송후 종료

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE 50

//소켓함수 오류 
void err_quit() {
	exit(1); //강제종료
}
void err_display() {
	printf("오류");
}

//제공해주는 recv대신에 사용자 정의 수신 함수 (recvn)
int recvn(SOCKET s, char* buf, int len, int flags) {
	int received; //수신변수
	char* ptr = buf; //버퍼의 시작 위치
	int left = len; //읽지 않은 길이 값 

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR) {
			return SOCKET_ERROR;
		}
		else if (received == 0) {
			break;
		}

		left -= received; //읽으면 읽지 않은 길이 감소 
		ptr += received;//읽으면 ptr증가
	}
	return (len - left); //전체-남은 자리수 ==> 읽은 값을 리턴 
}

int main() {
	int retval; //클라이언트 연결 처리 결과 

	//1> 윈속 초기화 
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		return 1;
	}

	//2> 소켓 생성
	/*SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		err_quit();
	}*/

	//3> connect ==> 서버ip, 포트 번호로 연결 시도 
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);//127.0.0.1
	serveraddr.sin_port = htons(SERVERPORT);
	

	//retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	//if (retval == SOCKET_ERROR) {
	//	err_quit();
	//}

	//4> send
	char buf[BUFSIZE];

	// 고정된 데이터 보내기 위한 배열 생성 배열 항목당 50사이즈 50-문자 나머지는 구분기호로 채움
	const char* testdata[] = {
		"안녕하세요",
		"반갑습니다",
		"컴퓨터공학과 20161222 이제연입니다",
		"잘 부탁드립니다"

	};

	int len;

	for (int i = 0; i < 4; i++)
	{
		// 매번 소캣 생성
		SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock == INVALID_SOCKET) {
			err_quit();
		}

		retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
		if (retval == SOCKET_ERROR) {
			err_quit();
		}

		len = strlen(testdata[i]);
		//버퍼에 배열의 값을 복사해서 넣어줌
		strncpy(buf, testdata[i], len);

		// bufsize 50으로 고정, 송신버퍼를 통해서 서버에 보내줌
		retval = send(sock, buf, len, 0);
		if (retval == SOCKET_ERROR) {
			err_display();
			break;
		}

		printf("TCP클라이언트%d 바이트보냈습니다\n", retval);

		closesocket(sock);

	}

	WSACleanup();
}