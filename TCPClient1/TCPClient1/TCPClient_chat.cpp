//TCPClient : IPV4

// 고정 + 가변길이 데이터 전송

// 메시지 입력 -> 서버에 보내기
// 1:1 방식

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

	const char* chatid = "클라이언트입니다 >>";


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

	//4> 데이터 통신에 사용할 변수 
	char msg[BUFSIZE+1];
	int len;
	char* buf;

	// 아이디 출력
	sprintf(msg, "[%s]", chatid);
	buf = msg + strlen(chatid);

	while(1)
	{
		// 보내기 위한 준비
		printf("%s", chatid);

		if (fgets(buf, BUFSIZE + 1, stdin) == NULL) 
			break;

		len = strlen(msg);
		if (msg[len - 1] = '\n') {
			msg[len - 1] = '\0';
		}

		if (strlen(buf) == 0) {
			break;
		}

		// bufsize 50으로 고정, 송신버퍼를 통해서 서버에 보내줌
		retval = send(sock, (char*)&len, sizeof(int), 0);	 // 고정길이 보내기
		if (retval == SOCKET_ERROR) {
			err_display();
			break;
		}

		retval = send(sock, buf, len, 0);			 // 가변길이 보내기
		if (retval == SOCKET_ERROR) {
			err_display();
			break;
		}

		// 데이터 받기
		retval = recvn(sock, (char*)&len, sizeof(int), 0); // 고정 길이 수신하기
		if (retval == INVALID_SOCKET) {
			err_display();
			break;
		}
		else if (retval == 0) break;

		retval = recvn(sock, buf, len, 0); // 가변 길이 수신하기
		if (retval == INVALID_SOCKET) {
			err_display();
			break;
		}
		else if (retval == 0) break;



		// 받은 데이터 출력
		buf[retval] - '\0';
		printf("%s\n", buf);

	}

	closesocket(sock);
	WSACleanup();
}