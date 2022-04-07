//TCPClient : IPV4

// ���� + �������� ������ ����

// �޽��� �Է� -> ������ ������
// 1:1 ���

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE 50

//�����Լ� ���� 
void err_quit() {
	exit(1); //��������
}
void err_display() {
	printf("����");
}

//�������ִ� recv��ſ� ����� ���� ���� �Լ� (recvn)
int recvn(SOCKET s, char* buf, int len, int flags) {
	int received; //���ź���
	char* ptr = buf; //������ ���� ��ġ
	int left = len; //���� ���� ���� �� 

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR) {
			return SOCKET_ERROR;
		}
		else if (received == 0) {
			break;
		}

		left -= received; //������ ���� ���� ���� ���� 
		ptr += received;//������ ptr����
	}
	return (len - left); //��ü-���� �ڸ��� ==> ���� ���� ���� 
}

int main() {
	int retval; //Ŭ���̾�Ʈ ���� ó�� ��� 

	const char* chatid = "Ŭ���̾�Ʈ�Դϴ� >>";


	//1> ���� �ʱ�ȭ 
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		return 1;
	}

	//2> ���� ����
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		err_quit();
	}

	//3> connect ==> ����ip, ��Ʈ ��ȣ�� ���� �õ� 
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);//127.0.0.1
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) {
		err_quit();
	}

	//4> ������ ��ſ� ����� ���� 
	char msg[BUFSIZE+1];
	int len;
	char* buf;

	// ���̵� ���
	sprintf(msg, "[%s]", chatid);
	buf = msg + strlen(chatid);

	while(1)
	{
		// ������ ���� �غ�
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

		// bufsize 50���� ����, �۽Ź��۸� ���ؼ� ������ ������
		retval = send(sock, (char*)&len, sizeof(int), 0);	 // �������� ������
		if (retval == SOCKET_ERROR) {
			err_display();
			break;
		}

		retval = send(sock, buf, len, 0);			 // �������� ������
		if (retval == SOCKET_ERROR) {
			err_display();
			break;
		}

		// ������ �ޱ�
		retval = recvn(sock, (char*)&len, sizeof(int), 0); // ���� ���� �����ϱ�
		if (retval == INVALID_SOCKET) {
			err_display();
			break;
		}
		else if (retval == 0) break;

		retval = recvn(sock, buf, len, 0); // ���� ���� �����ϱ�
		if (retval == INVALID_SOCKET) {
			err_display();
			break;
		}
		else if (retval == 0) break;



		// ���� ������ ���
		buf[retval] - '\0';
		printf("%s\n", buf);

	}

	closesocket(sock);
	WSACleanup();
}