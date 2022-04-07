// ���̺귯�� �߰��Ǹ� ���� ����
//#pragma commnet(lib,"ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERPORT 9000
#define BUFSIZE 512

// ���� �Լ� ���� ��� �� ����
void err_quit(const char* msg) {
	exit(1); // ��������
}

// ���� ���� �޽��� ���
void err_display(const char* msg) {
	printf("����");
}

int main() {
	int retval; // ���� ���� ������ ���õ� ���� ==> bind, listen ó�� ��� ����

	// 1> ���� �ʱ�ȭ 
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// 2> ��Ĺ ����( ���� )
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0); // socket(�ּ�ü��, ����Ÿ��, ��������)
	if (listen_sock == INVALID_SOCKET) {
		err_quit("socket ����");
	}
	
	// 3> ���� ip�ּ�, ��Ʈ ��ȣ ����(bind)
	SOCKADDR_IN serveraddr; // ��������ü ����
	ZeroMemory(&serveraddr, sizeof(serveraddr)); // �����ּ� ������ 0���� �ʱ�ȭ
	serveraddr.sin_family = AF_INET; // �ּ�ü��(IPv4, IPv6)
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); // IP�ּ� ���� htonl : ȣ��Ʈ -> ��Ʈ��ũ ����Ʈ ����
													// INADDR_ANY : �ڵ����� ip�ּ� ����
	serveraddr.sin_port = htons(SERVERPORT); // ȣ��Ʈ -> ��Ʈ��ũ ����Ʈ��
	
	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr)); //bind(����, �ּ�, ũ��)
	if (retval == SOCKET_ERROR) {
		err_quit("bind ����");
	}


	// 4> ���� �����ϰ� Ŭ���̾�Ʈ ���� �غ��ϰ� ��� ���� : listen() 
	retval = listen(listen_sock, SOMAXCONN); // �ִ밪���� ������ �غ� listen(����, ���� Ŭ���̾�Ʈ ����)
	if (retval == SOCKET_ERROR) {
		err_quit("listen ����");
	}

	// ------------- ���� �غ� ----------------

	// ������ �̷�������� ����� ���� ����
	SOCKET client_sock;		// ���� ����
	SOCKADDR_IN clientaddr; // ����� Ŭ���̾�Ʈ �ּ�
	int addrlen;			// �ּ� ����
	char buf[BUFSIZE + 1];  // ������ �������

	while (1) {
		addrlen = sizeof(clientaddr);

		//Ŭ���̾�Ʈ�� ������ �޾� �帮�� ������ ����
		// accept(���� ����, Ŭ���̾�Ʈ �ּ�, ����)
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen); 
		if (client_sock == INVALID_SOCKET) {
			err_display("accept ����");
			break;
		}
		
		//������ Ŭ���̾�Ʈ ���� ���
		printf("\n[TCP ���� Ŭ���̾�Ʈ ���� : IP�ּ� = %s, ��Ʈ ��ȣ = %d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		//Ŭ���̾�Ʈ���� ���
		while (1) {
			//recv(����, ���� ����, ũ�� , 0) �����͸� �о����
			retval = recv(client_sock, buf, BUFSIZE, 0);
			if (retval == INVALID_SOCKET) {
				err_display("recv() ����");
				break;
			}
			else if (retval == 0) break;


			// ���� ������ ���
			buf[retval] = '\0'; // ���ۿ� ���� ǥ��
			printf("[TCP%s:%d] %s \n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), buf);

			// ������ ������
			retval = send(client_sock, buf, retval, 0);
			if (retval == INVALID_SOCKET) {
				err_display("send() ����");
				break;
			}
		}

		// Ŭ���̾�Ʈ ������ �ְ�ޱ� ������
		closesocket(client_sock);

		printf("\n[TCP ���� Ŭ���̾�Ʈ ���� : IP�ּ� = %s, ��Ʈ ��ȣ = %d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	}

	// accept ������
	closesocket(listen_sock);

	// ���� ������
	WSACleanup();

}