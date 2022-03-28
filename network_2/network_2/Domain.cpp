
#pragma comment(lib, "ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>
#include <string.h>

#define TESTNAME "www.example.com"

//������ -> IP�ּ� 
BOOL GetIPAddr(const char* name, IN_ADDR* addr) {

	HOSTENT* ptr = gethostbyname(name);
	if (ptr == NULL) {
		printf("����");
		return FALSE;
	}

	if (ptr->h_addrtype != AF_INET) {
		printf("����");
		return FALSE;
	}

	memcpy(addr, ptr->h_addr, ptr->h_length);
	return TRUE;
}
//ip->������ 
BOOL GetDomainName(IN_ADDR addr, char* name, int namelen) {

	HOSTENT* ptr = gethostbyaddr((char*)&addr, sizeof(addr), AF_INET);
	if (ptr == NULL) {
		printf("����");
		return FALSE;
	}

	if (ptr->h_addrtype != AF_INET) {
		printf("����");
		return FALSE;
	}

	memcpy(name, ptr->h_name, namelen);
	return TRUE;
}

int main() {

	WSADATA wsa; //���� �ʱ�ȭ 

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		return 1;
	}

	printf("������ �̸� ��ȯ��=%s\n", TESTNAME);

	//������ -> ip
	IN_ADDR addr;
	if (GetIPAddr(TESTNAME, &addr)) {
		printf("IP�ּ� ��ȯ �� = %s\n", inet_ntoa(addr));
	}

	//ip->������
	char name[256];
	if (GetDomainName(addr, name, sizeof(name))) {
		printf("������ ��ȭ �� =%s\n", name);
	}

}
