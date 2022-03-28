
#pragma comment(lib, "ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>
#include <string.h>

#define TESTNAME "www.example.com"

//도메인 -> IP주소 
BOOL GetIPAddr(const char* name, IN_ADDR* addr) {

	HOSTENT* ptr = gethostbyname(name);
	if (ptr == NULL) {
		printf("오류");
		return FALSE;
	}

	if (ptr->h_addrtype != AF_INET) {
		printf("오류");
		return FALSE;
	}

	memcpy(addr, ptr->h_addr, ptr->h_length);
	return TRUE;
}
//ip->도메인 
BOOL GetDomainName(IN_ADDR addr, char* name, int namelen) {

	HOSTENT* ptr = gethostbyaddr((char*)&addr, sizeof(addr), AF_INET);
	if (ptr == NULL) {
		printf("오류");
		return FALSE;
	}

	if (ptr->h_addrtype != AF_INET) {
		printf("오류");
		return FALSE;
	}

	memcpy(name, ptr->h_name, namelen);
	return TRUE;
}

int main() {

	WSADATA wsa; //윈속 초기화 

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		return 1;
	}

	printf("도메인 이름 변환전=%s\n", TESTNAME);

	//도메인 -> ip
	IN_ADDR addr;
	if (GetIPAddr(TESTNAME, &addr)) {
		printf("IP주소 변환 후 = %s\n", inet_ntoa(addr));
	}

	//ip->도메인
	char name[256];
	if (GetDomainName(addr, name, sizeof(name))) {
		printf("도메인 변화 후 =%s\n", name);
	}

}
