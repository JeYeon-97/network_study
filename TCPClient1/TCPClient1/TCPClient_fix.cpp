//TCPClient : IPV4
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

	//4> send
	char buf[BUFSIZE];

	// ������ ������ ������ ���� �迭 ���� �迭 �׸�� 50������ 50-���� �������� ���б�ȣ�� ä��
	const char* testdata[] = {
		"�ȳ��ϼ���",
		"�ݰ����ϴ�",
		"��ǻ�Ͱ��а� 20161222 �������Դϴ�",
		"�� ��Ź�帳�ϴ�"

	};

	int len;

	//while (1) {
	//	printf("\n[����������]: ");
	//	if (fgets(buf, BUFSIZE + 1, stdin) == NULL) {
	//		break;
	//	}
	//	len = strlen(buf); //�Էµ� ���� ���� 
	//	if (buf[len - 1] == '\n') {
	//		buf[len - 1] = '\0';//enter�� ������ enter �ڸ��� 0���� ó��
	//	}
	//	if (strlen(buf) == 0) {
	//		break;
	//	}

	for (int i = 0; i<4; i++)
	{
		// memset(��ġ, ���б�ȣ, ũ��);
		memset(buf, '#', sizeof(buf)); 

		//���ۿ� �迭�� ���� �����ؼ� �־���
		strncpy(buf, testdata[i], strlen(testdata[i]));

		// bufsize 50���� ����, �۽Ź��۸� ���ؼ� ������ ������
		retval = send(sock, buf, BUFSIZE, 0); 
		if (retval == SOCKET_ERROR) {
			err_display();
			break;
		}
		printf("TCPŬ���̾�Ʈ%d ����Ʈ���½��ϴ�\n", retval);

		////5> recv
		//retval = recvn(sock, buf, retval, 0); //����� ���� �����Լ�
		//if (retval == SOCKET_ERROR) {
		//	err_display();
		//	break;
		//}
		//else if (retval == 0) {
		//	break;
		//}

		//buf[retval] = '\0';
		//printf("TCPŬ���̾�Ʈ%d ����Ʈ����\n", retval);
		//printf("���������� %s\n", buf);
	}

	closesocket(sock);
	WSACleanup();
}