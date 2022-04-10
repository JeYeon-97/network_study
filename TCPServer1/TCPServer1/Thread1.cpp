// 스레드 생성 + 종료
// 스레드에 사용될 함수를 정의
// createThread() 함수를 통해서 호출하면서 실행
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>

// 구조체
struct Point3D {
	int x, y, z;
};

// 스레드 작업을 할 함수를 정의
DWORD WINAPI MyThread(LPVOID arg) {

	Point3D* pt = (Point3D*)arg;	// 매개변수값을 구조체로 형변환
	while (1) {
		printf("running thread() %d, %d, %d, %d \n", GetCurrentThreadId(), pt->x, pt->y, pt->z);
		
		Sleep(1000); // 1초 간격으로 잠깐 멈춤
	}
	return 0;
}

int main() {
	// 스레드 생성
	Point3D pt1 = { 10,20,30 };
	// 보안, 크기 ,스레드함수 , 매개변수, 실행여부, 스레드id
	HANDLE hThread1 = CreateThread(NULL, 0, MyThread, &pt1,0, NULL);
	if (hThread1 == NULL) {
		return 1;
	}
	CloseHandle(hThread1); // 스레드 닫기

	// 스레드 생성
	Point3D pt2 = { 40,50,60 };
	// 보안, 크기 ,스레드함수 , 매개변수, 실행여부, 스레드id
	HANDLE hThread2 = CreateThread(NULL, 0, MyThread, &pt2, 0, NULL);
	if (hThread2 == NULL) {
		return 1;
	}
	CloseHandle(hThread2);	 // 스레드 닫기


	//메인(주쓰레드) --> 스레드1 --> 스레드2 반복

	while (1) {
		printf("run main %d\n",GetCurrentThreadId());
		Sleep(1000);
	}

}