/*
	CriticalSectionSync.cpp
	프로그램 설명: 생성 가능한 쓰레드의 개수 측정.
*/

#include <stdio.h>
#include <windows.h>
#include <process.h>
#include <tchar.h>

#define NUM_OF_GATE		5

LONG gTotal = 0;
int data[12];
int cnt = 0;

CRITICAL_SECTION   hCriticalSection;

void Input() {
	for (int i = 0; i < 12; i++)
	{
		scanf_s("%d ", &data[i]);
	}
	cnt++;
}

void Add() {
	int inital = (cnt - 1) * 4;
	for (DWORD i = inital; i <= inital + 3; i++)
	{
		gTotal += data[i];
	}
	cnt++;
}

void Output() {
	_tprintf(_T("total count: %d \n"), gTotal);
}

unsigned int WINAPI ThreadProc( LPVOID lpParam ) 
{ 
	EnterCriticalSection(&hCriticalSection);
	if (cnt == 0) {
		Input();
	}
	else if (cnt == 4) {
		Output();
	}
	else {
		Add();
	}
	LeaveCriticalSection(&hCriticalSection);

	return 0;
} 

int _tmain(int argc, TCHAR* argv[])
{
    DWORD dwThreadId[NUM_OF_GATE];
    HANDLE hThread[NUM_OF_GATE];

	InitializeCriticalSection(&hCriticalSection);

	for (DWORD i = 0; i < NUM_OF_GATE; i++)
	{
		hThread[i] = (HANDLE)
			_beginthreadex(
				NULL,
				0,
				ThreadProc,
				NULL,
				CREATE_SUSPENDED,
				(unsigned*)&dwThreadId[i]
			);

		if (hThread[i] == NULL)
		{
			_tprintf(_T("Thread creation fault! \n"));
			return -1;
		}
	}

	for (DWORD i = 0; i < NUM_OF_GATE; i++)
	{
		ResumeThread(hThread[i]);
	}

	WaitForMultipleObjects(NUM_OF_GATE, hThread, TRUE, INFINITE);

	for(DWORD i=0; i<NUM_OF_GATE; i++)
	{
		CloseHandle(hThread[i]);
	}

	DeleteCriticalSection(&hCriticalSection);

	return 0;
}
