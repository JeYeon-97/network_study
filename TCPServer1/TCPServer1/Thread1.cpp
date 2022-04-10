// ������ ���� + ����
// �����忡 ���� �Լ��� ����
// createThread() �Լ��� ���ؼ� ȣ���ϸ鼭 ����
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>

// ����ü
struct Point3D {
	int x, y, z;
};

// ������ �۾��� �� �Լ��� ����
DWORD WINAPI MyThread(LPVOID arg) {

	Point3D* pt = (Point3D*)arg;	// �Ű��������� ����ü�� ����ȯ
	while (1) {
		printf("running thread() %d, %d, %d, %d \n", GetCurrentThreadId(), pt->x, pt->y, pt->z);
		
		Sleep(1000); // 1�� �������� ��� ����
	}
	return 0;
}

int main() {
	// ������ ����
	Point3D pt1 = { 10,20,30 };
	// ����, ũ�� ,�������Լ� , �Ű�����, ���࿩��, ������id
	HANDLE hThread1 = CreateThread(NULL, 0, MyThread, &pt1,0, NULL);
	if (hThread1 == NULL) {
		return 1;
	}
	CloseHandle(hThread1); // ������ �ݱ�

	// ������ ����
	Point3D pt2 = { 40,50,60 };
	// ����, ũ�� ,�������Լ� , �Ű�����, ���࿩��, ������id
	HANDLE hThread2 = CreateThread(NULL, 0, MyThread, &pt2, 0, NULL);
	if (hThread2 == NULL) {
		return 1;
	}
	CloseHandle(hThread2);	 // ������ �ݱ�


	//����(�־�����) --> ������1 --> ������2 �ݺ�

	while (1) {
		printf("run main %d\n",GetCurrentThreadId());
		Sleep(1000);
	}

}