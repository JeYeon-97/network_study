//#define _CRT
#include <iostream>
#include <stdio.h>
#include <WinSock2.h>
#include <Windows.h>

void err_display(char* msg) {
    printf("오류");
}


int main()
{
    WSADATA wsa; // 윈속 초기화

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        return 1;
    }

    MessageBox(NULL, "윈속 초기화 성공 ", "알림", MB_OK);

    

    SOCKET tcp_sock = socket(AF_INET, SOCK_STREAM, 0); //(주소체계, 소켓타입, 프로토콜)

    if (tcp_sock == INVALID_SOCKET) {
        err_display((char*)"socket()");
    }
    MessageBox(NULL, "TCP 소켓 성공", "알림", MB_OK);

    closesocket(tcp_sock);

    WSACleanup(); // 적재한 DLL을 메모리에서 해제하는 함수
}
