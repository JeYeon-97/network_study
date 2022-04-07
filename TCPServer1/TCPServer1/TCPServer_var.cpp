// 라이브러리 추가되면 생략 가능
//#pragma commnet(lib,"ws2_32")

// TCP 서버 역할 : 가변 길이
// 가변길이는 메시지의 끝을 표현 해야한다
// 전체 메시지를 읽어온 후에 한 글자씩 리턴하는 형식 


#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERPORT 9000
#define BUFSIZE 512

// 소켓 함수 오류 출력 및 종료
void err_quit(const char* msg) {
	exit(1); // 강제종료
}

// 소켓 오류 메시지 출력
void err_display(const char* msg) {
	printf("오류");
}

// 함수 정의
int _recv_head(SOCKET s, char* p) {
	// 수신 버퍼에 있는 내용을 한 글자씩 리턴해주는 역할
	static int nbytes = 0;	// 읽은 바이트 (공유변수)
	static char buf[1024];
	static char* ptr;		// 읽을 위치


	// 소켓 수신 버퍼에 읽어드린 데이터가 없거나 모두 읽어서 비워져 있는 경우
	if (nbytes == 0 || nbytes == SOCKET_ERROR) {	
		nbytes = recv(s, buf, sizeof(buf), 0);    // 다시 읽어들임

		if (nbytes == SOCKET_ERROR) return SOCKET_ERROR;  
		else if (nbytes == 0) return 0;
		
		ptr = buf;  //버퍼의 시작위치
	}
	--nbytes;		//남은 버퍼 공간 1씩 감소
	*p = *ptr++;	//포인터는 증가

	return 1;		// 한 바이트씩 읽었을때 1을 리턴
}

// 전체 메시지를 읽기 위해서 사용되는 함수
int recvLine(SOCKET s, char* buf, int maxlen) {
	int n;		// 전체 길이 체크할 변수
	int nbytes; // _recv_head()로부터 리턴 받은 값
	char c;     // 읽을 문자
	char *ptr = buf;	// 버퍼위치

	for (n = 1; n < maxlen; n++) {
		nbytes = _recv_head(s, &c);

		if (nbytes == 1) {
			*ptr++ = c;

			// 가져온 문자가 줄바꿈이면 전체 다 읽었으므로 중단 ===> n이 나올때까지 데이터를 읽어옴
			if (c == '\n') break;		
		}
		else if (nbytes == 0) {
			*ptr = 0;
			return n - 1;
		}
		else {
			return SOCKET_ERROR;
		}
	}

	*ptr = 0;
	return n;

}


int main() {
	int retval; // 서버 연결 관리와 관련된 변수 ==> bind, listen 처리 결과 관리

	// 1> 윈속 초기화 
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// 2> 소캣 생성( 서버 )
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0); // socket(주소체계, 소켓타입, 프로토콜)
	if (listen_sock == INVALID_SOCKET) {
		err_quit("socket 오류");
	}

	// 3> 서버 ip주소, 포트 번호 설정(bind)
	SOCKADDR_IN serveraddr; // 서버구조체 변수
	ZeroMemory(&serveraddr, sizeof(serveraddr)); // 서버주소 변수를 0으로 초기화
	serveraddr.sin_family = AF_INET; // 주소체계(IPv4, IPv6)
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); // IP주소 설정 htonl : 호스트 -> 네트워크 바이트 정렬
													// INADDR_ANY : 자동으로 ip주소 설정
	serveraddr.sin_port = htons(SERVERPORT); // 호스트 -> 네트워크 바이트로

	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr)); //bind(소켓, 주소, 크기)
	if (retval == SOCKET_ERROR) {
		err_quit("bind 오류");
	}


	// 4> 서버 실행하고 클라이언트 받을 준비하고 대기 상태 : listen() 
	retval = listen(listen_sock, SOMAXCONN); // 최대값으로 연결할 준비 listen(소켓, 연결 클라이언트 갯수)
	if (retval == SOCKET_ERROR) {
		err_quit("listen 오류");
	}

	// ------------- 연결 준비 ----------------

	// 연결이 이루어졌을때 사용할 변수 셋팅
	SOCKET client_sock;		// 연결 소켓
	SOCKADDR_IN clientaddr; // 연결된 클라이언트 주소
	int addrlen;			// 주소 길이
	char buf[BUFSIZE + 1];  // 데이터 저장공간

	while (1) {
		addrlen = sizeof(clientaddr);

		//클라이언트의 연결을 받아 드리고 소켓을 생성
		// accept(서버 소켓, 클라이언트 주소, 길이)
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept 오류");
			break;
		}

		//접속한 클라이언트 정보 출력
		printf("\n[TCP 서버 클라이언트 접속 : IP주소 = %s, 포트 번호 = %d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
		


		//클라이언트와의 통신
		while (1) {

			//recv(소켓, 읽을 공간, 크기 , 0) 데이터를 읽어들임
			retval = recvLine(client_sock, buf, BUFSIZE+1);

			if (retval == INVALID_SOCKET) {
				err_display("recvLine() 오류");
				break;
			}
			else if (retval == 0) break;


			// 받은 데이터 출력
			//buf[retval] = '\0'; // 버퍼에 끝을 표시
			printf("[TCP%s:%d] %s \n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), buf);

			
		}

		// 클라이언트 데이터 주고받기 마무리
		closesocket(client_sock);

		printf("\n[TCP 서버 클라이언트 종료 : IP주소 = %s, 포트 번호 = %d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	}

	// accept 마무리
	closesocket(listen_sock);

	// 윈속 마무리
	WSACleanup();

}