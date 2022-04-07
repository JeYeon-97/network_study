// ���̺귯�� �߰��Ǹ� ���� ����
//#pragma commnet(lib,"ws2_32")

// TCP ���� ���� : ���� ����
// �������̴� �޽����� ���� ǥ�� �ؾ��Ѵ�
// ��ü �޽����� �о�� �Ŀ� �� ���ھ� �����ϴ� ���� 


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

// �Լ� ����
int _recv_head(SOCKET s, char* p) {
	// ���� ���ۿ� �ִ� ������ �� ���ھ� �������ִ� ����
	static int nbytes = 0;	// ���� ����Ʈ (��������)
	static char buf[1024];
	static char* ptr;		// ���� ��ġ


	// ���� ���� ���ۿ� �о�帰 �����Ͱ� ���ų� ��� �о ����� �ִ� ���
	if (nbytes == 0 || nbytes == SOCKET_ERROR) {	
		nbytes = recv(s, buf, sizeof(buf), 0);    // �ٽ� �о����

		if (nbytes == SOCKET_ERROR) return SOCKET_ERROR;  
		else if (nbytes == 0) return 0;
		
		ptr = buf;  //������ ������ġ
	}
	--nbytes;		//���� ���� ���� 1�� ����
	*p = *ptr++;	//�����ʹ� ����

	return 1;		// �� ����Ʈ�� �о����� 1�� ����
}

// ��ü �޽����� �б� ���ؼ� ���Ǵ� �Լ�
int recvLine(SOCKET s, char* buf, int maxlen) {
	int n;		// ��ü ���� üũ�� ����
	int nbytes; // _recv_head()�κ��� ���� ���� ��
	char c;     // ���� ����
	char *ptr = buf;	// ������ġ

	for (n = 1; n < maxlen; n++) {
		nbytes = _recv_head(s, &c);

		if (nbytes == 1) {
			*ptr++ = c;

			// ������ ���ڰ� �ٹٲ��̸� ��ü �� �о����Ƿ� �ߴ� ===> n�� ���ö����� �����͸� �о��
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
			retval = recvLine(client_sock, buf, BUFSIZE+1);

			if (retval == INVALID_SOCKET) {
				err_display("recvLine() ����");
				break;
			}
			else if (retval == 0) break;


			// ���� ������ ���
			//buf[retval] = '\0'; // ���ۿ� ���� ǥ��
			printf("[TCP%s:%d] %s \n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), buf);

			
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