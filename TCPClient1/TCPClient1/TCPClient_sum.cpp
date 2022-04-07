//TCPClient : IPV4
// Ŭ���̾�Ʈ���� 2���� ��� �Է��� ���� �ʿ��� ���
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE 512

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

	//3> connect==> ����ip, ��Ʈ ��ȣ�� ���� �õ� 
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
	char buf[BUFSIZE + 1];
	int len;

	// �޽��� ����
	struct Mydata {
		int n1;
		int n2;
	}data;

	int result;
	int dummy; // ���� üũ


	while (1) {
		printf("\n[2���� ���� �Է�]: ");
		if (fgets(buf, BUFSIZE + 1, stdin) == NULL) {
			break;
		}
		// ����ó��
		len = strlen(buf); //�Էµ� ���� ���� 
		if (buf[len - 1] == '\n') {
			buf[len - 1] = '\0';//enter�� ������ enter �ڸ��� 0���� ó��
		}
		if (strlen(buf) == 0) {
			break;
		}

		// ����üũ
		if (sscanf(buf, " %d%d%d", &data.n1, &data.n2, &dummy) != 2) {
			fprintf(stderr, "�����޽���");
			continue;
		}


		retval = send(sock, (char *)&data, sizeof(data), 0);
		if (retval == SOCKET_ERROR) {
			err_display();
			break;
		}
		

		//5> recv ������ �ޱ�
		retval = recvn(sock, (char *)&result, sizeof(result), 0); //����� ���� �����Լ�
		if (retval == SOCKET_ERROR) {
			err_display();
			break;
		}
		else if (retval == 0) {
			break;
		}

		buf[retval] = '\0';
		printf("���������� %d + %d = %d \n",data.n1, data.n2, result);
	}

	closesocket(sock);
	WSACleanup();
}