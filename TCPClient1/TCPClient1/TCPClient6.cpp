// TCP Ŭ���̾�Ʈ
//#pragma commnet(lib,"ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <WS2tcpip.h>

#define SERVERIP "::1" // 0:0:0:0:0:0:0:1
#define SERVERPORT 9000
#define BUFSIZE 512

// ���� �Լ� ����
void err_quit(const char* msg) {
	exit(1); //�������� 
}

// ���� �Լ� ���� ���
void err_display(const char* msg) {
	printf("����");
}

// ������ ���� �Լ�( ����� ���� �Լ�)
int recvn(SOCKET s, char* buf, int len, int flags) {
	int received; // ���� ������ ó�� ����

	char* ptr = buf; // ���� ������ ���� ��ġ
	int left = len;  // ���� ������ ���� ���� ���� ��

	while (left > 0) {
		received = recv(s, ptr, left, flags);

		if (received == SOCKET_ERROR) {
			return SOCKET_ERROR;
		}
		else if (received == 0) {
			break;
		}

		left -= received;  // ���� ���� ��
		ptr += received;   // ���� ��
	}
	return (len - left); // ���� ���� �� ����
}
int main() {
	int retval;

	// 1> ���� �ʱ�ȭ 
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	// 2> ���� ���� 
	SOCKET sock = socket(AF_INET6, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		err_quit("socket() ����");
	}

	// 3> connect (���� ip, ��Ʈ ��ȣ)
	SOCKADDR_IN6 serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin6_family = AF_INET6;

	int addrlen = sizeof(serveraddr);
	WSAStringToAddress((LPSTR)SERVERIP, AF_INET6, NULL, (SOCKADDR*)&serveraddr, &addrlen);

	serveraddr.sin6_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));

	if (retval == SOCKET_ERROR) {
		err_quit("connect() ����");
	}

	// 4> ������ ��ſ� ����� ����
	char buf[BUFSIZE + 1];
	int len;

	while (1) {
		printf("\n[���� ������] : ");

		if (fgets(buf, BUFSIZE + 1, stdin) == NULL) {
			break;
		}
		len = strlen(buf); // ����Ű ���ֱ�?
		if (buf[len - 1] == '\n') {
			buf[len - 1] = '\0';
		}
		if (strlen(buf) == 0) {
			break;
		}

		// ������
		retval = send(sock, buf, strlen(buf), 0); // �۽Ź��۸� ���ؼ� ������ ������
		if (retval == SOCKET_ERROR) {
			err_display("send()����");
			break;
		}

		printf("[TCP Ŭ���̾�Ʈ] %d ����Ʈ�� ���½��ϴ� \n", retval);

		// ������ �ޱ� 
		retval == recvn(sock, buf, retval, 0);
		if (retval == SOCKET_ERROR) {
			err_display("recvn()����");
			break;
		}
		else if (retval == 0) {
			break;
		}

		buf[retval] = '\0';
		printf("[TCP Ŭ���̾�Ʈ] %d ����Ʈ�� �޾ҽ��ϴ�.\n", retval);
		printf("[���� ������] %s \n", buf);


	}

	closesocket(sock);

	WSACleanup();

}