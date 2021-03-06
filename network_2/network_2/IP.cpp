#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <WinSock2.h>
#include <Windows.h>
#include <WS2tcpip.h>

int main() {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    // -----IPv4 변환 연습-------

    const char* ipv4test = "147.46.114.70";
    printf("IPv4 주소(변환 전) = %s\n", ipv4test);

    // inet_addr()함수 연습
    printf("IPv4 주소(변환 후) = 0x%x\n", inet_addr(ipv4test)); // 문자열 -> 32비트 정수

    // inet_ntoa()함수 연습
    IN_ADDR ipv4num;
    ipv4num.s_addr = inet_addr(ipv4test);
    printf("IPv4 주소(다시 변환 후) = %s\n", inet_ntoa(ipv4num)); // 32비트 정수 -> 문자열

    printf("\n");


    // -----IPv6 변환 연습-------

    //원래의 IPv6 주소 출력
    const char * ipv6test = "2001:0230:abcd:ffab:0023:eb00:ffff:1111";
    printf("IPv6 주소(변환 전) = %s\n", ipv6test);

    // WSAStringToAddress() 함수 연습 (문자열 -> 숫자)
    SOCKADDR_IN6 ipv6num;
    int addrlen = sizeof(ipv6num);

    
    WSAStringToAddress((LPSTR)ipv6test, AF_INET6, NULL,(SOCKADDR*)&ipv6num,&addrlen);

    printf("IPv6 주소(변환 후) = 0x");
    for (int i = 0; i < 16; i++)
        printf("%02x", ipv6num.sin6_addr.u.Byte[i]);
    printf("\n");

    // WSAAddressToString() 함수 연습 (숫자 -> 문자열)
    char ipaddr[50]; // ip주소(문자열)을 저장할 버퍼
    DWORD ipaddrlen = sizeof(ipaddr);

    WSAAddressToString((SOCKADDR*)&ipv6num, sizeof(ipv6num), NULL, ipaddr, &ipaddrlen);

    printf("IPv6 주소(다시 변환 후) = %s\n", ipaddr);

    WSACleanup();
    return 0;




}