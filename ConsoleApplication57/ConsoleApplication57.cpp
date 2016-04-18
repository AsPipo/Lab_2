// ConsoleApplication57.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <windows.h>
#define ITERATIONS_NUMBER 50
#define PAUSE 10 /* ms */

#include <intrin.h>
#include <winnt.h>

using namespace std;

int N=100;
int arr[100];
long volatile sum = 0;
int i=0;

CRITICAL_SECTION sync;

struct IterationsSettings
{
	int startFrom;
	int goTo;
};

DWORD WINAPI ThreadProc(CONST LPVOID lpParam) 
{
  	IterationsSettings loopLimits = *(IterationsSettings*)lpParam;
	long local_sum = 0;
	for(i = loopLimits.startFrom; i < loopLimits.goTo; i++) 
	{
		local_sum += arr[i]; 
	}
	
	EnterCriticalSection(&sync);

	sum += local_sum;

	LeaveCriticalSection(&sync);

	return 0;
}
int _tmain(int argc, _TCHAR* argv[])
{
	for ( int i = 0; i < N; i++ )
		arr[i] = i;  

	InitializeCriticalSection(&sync);

	IterationsSettings loopLimits1;
	loopLimits1.startFrom = 0;
	loopLimits1.goTo = N/2;
	IterationsSettings loopLimits2;
	loopLimits2.startFrom = N/2;
	loopLimits2.goTo = N;

	HANDLE hThreads[2];

	hThreads[0] = CreateThread(NULL,NULL,ThreadProc,&loopLimits1,0,0);

	hThreads[1] = CreateThread(NULL,NULL,ThreadProc,&loopLimits2,0,0);

	WaitForMultipleObjects(2, hThreads, TRUE, INFINITE);

 /* DWORD dwTemp, i;
  HANDLE hThreads[THREADS_NUMBER];
  CONST HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
  CONST HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL); 
  WaitForMultipleObjects(THREADS_NUMBER, hThreads, TRUE, INFINITE);
  wsprintf(szMessage, TEXT("sum = %d\r\n"), Sum);

  	for(i = 0; i < THREADS_NUMBER; i++) 
	{
		CloseHandle(hThreads[i]);
	}
	CloseHandle(hEvent);
  	return 0;*/
	cout<<sum;
	system("pause");
}

