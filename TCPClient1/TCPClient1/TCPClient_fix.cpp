//TCPClient : IPV4
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
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		err_quit();
	}

	//3> connect ==> 서버ip, 포트 번호로 연결 시도 
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);//127.0.0.1
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) {
		err_quit();
	}

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

	//while (1) {
	//	printf("\n[보낼데이터]: ");
	//	if (fgets(buf, BUFSIZE + 1, stdin) == NULL) {
	//		break;
	//	}
	//	len = strlen(buf); //입력된 값의 길이 
	//	if (buf[len - 1] == '\n') {
	//		buf[len - 1] = '\0';//enter가 들어오면 enter 자리에 0으로 처리
	//	}
	//	if (strlen(buf) == 0) {
	//		break;
	//	}

	for (int i = 0; i<4; i++)
	{
		// memset(위치, 구분기호, 크기);
		memset(buf, '#', sizeof(buf)); 

		//버퍼에 배열의 값을 복사해서 넣어줌
		strncpy(buf, testdata[i], strlen(testdata[i]));

		// bufsize 50으로 고정, 송신버퍼를 통해서 서버에 보내줌
		retval = send(sock, buf, BUFSIZE, 0); 
		if (retval == SOCKET_ERROR) {
			err_display();
			break;
		}
		printf("TCP클라이언트%d 바이트보냈습니다\n", retval);

		////5> recv
		//retval = recvn(sock, buf, retval, 0); //사용자 정의 수신함수
		//if (retval == SOCKET_ERROR) {
		//	err_display();
		//	break;
		//}
		//else if (retval == 0) {
		//	break;
		//}

		//buf[retval] = '\0';
		//printf("TCP클라이언트%d 바이트받음\n", retval);
		//printf("받은데이터 %s\n", buf);
	}

	closesocket(sock);
	WSACleanup();
}