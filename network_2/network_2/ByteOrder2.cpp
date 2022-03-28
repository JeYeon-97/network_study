#include <iostream>
#include <stdio.h>
#include <WinSock2.h>
#include <Windows.h>


int main()
{
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

    u_short x1 = 0x1234;
    u_long y1 = 0x12345678;
    u_short x2, x3;
    u_long y2, y3;

    printf("[호스트 바이트 -> 네트워크 바이트] \n");
    WSAHtons(sock, x1, &x2);
    printf("0x%x -> 0x%x\n", x1,x2);
    WSAHtonl(sock, y1, &y2);
    printf("0x%x -> 0x%x\n", y1,y2);

    printf("[네트워크 바이트 -> 호스트 바이트] \n");
    WSANtohs(sock, x2, &x3);
    printf("0x%x -> 0x%x\n", x2,x3);
    WSANtohl(sock, y2, &y3);
    printf("0x%x -> 0x%x\n", y2,y3);

    WSACleanup();

    return 0;
}