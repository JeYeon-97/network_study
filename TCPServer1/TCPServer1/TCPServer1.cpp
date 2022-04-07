// 라이브러리 추가되면 생략 가능
//#pragma commnet(lib,"ws2_32")
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
			retval = recv(client_sock, buf, BUFSIZE, 0);
			if (retval == INVALID_SOCKET) {
				err_display("recv() 오류");
				break;
			}
			else if (retval == 0) break;


			// 받은 데이터 출력
			buf[retval] = '\0'; // 버퍼에 끝을 표시
			printf("[TCP%s:%d] %s \n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), buf);

			// 데이터 보내기
			retval = send(client_sock, buf, retval, 0);
			if (retval == INVALID_SOCKET) {
				err_display("send() 오류");
				break;
			}
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