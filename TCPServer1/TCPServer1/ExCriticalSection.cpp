#include <Windows.h>
#include <stdio.h>

#define MAXCNT 10000000
int g_count = 0;
CRITICAL_SECTION cs;

DWORD WINAPI MyThread1(LPVOID arg) {
	
	EnterCriticalSection(&cs);
	g_count += 2;
	LeaveCriticalSection(&cs);
	
	return 0;
	
}

DWORD WINAPI MyThread2(LPVOID arg) {
	// �Ӱ迵���� ��
	EnterCriticalSection(&cs);
	g_count += 2;

	// �Ӱ迵�� ���
	LeaveCriticalSection(&cs);

	return 0;
	
}

int main(int argc, char* argv[]) {

	// 1. �Ӱ� ���� �ʱ�ȭ
	InitializeCriticalSection(&cs);

	// �� �̻��� �����带 ����
	HANDLE hThread[2];
	hThread[0] = CreateThread(NULL, 0, MyThread1, NULL, 0, NULL);
	hThread[1] = CreateThread(NULL, 0, MyThread2, NULL, 0, NULL);
	
	// �ΰ��� ������ ���� ���
	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);

	// �Ӱ� ���� ����
	DeleteCriticalSection(&cs);

	// ��� ���
	printf("g_count = %d \n", g_count);
	return 0;


}