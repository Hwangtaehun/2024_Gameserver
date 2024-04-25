#include "stdafx.h"
#include "Windows.h"
#include "iostream"
using namespace std;

int data[12];
int total = 0;
int index = 0;

DWORD WINAPI ThreadProc(LPVOID pParam)
{
	HANDLE hEvent = (HANDLE)pParam;

	DWORD dwWaitCode = WaitForSingleObject(hEvent, INFINITE);
	if (dwWaitCode == WAIT_FAILED)
	{
		cout << "WaitForSingleObject failed : " << GetLastError() << endl;
		return 0;
	}

	for (int i = index; i < index + 4; i++) {
		total += ::data[i];
	}
	index += 4;
	
	SetEvent(hEvent);
	return 0;
}

void _tmain(void)
{
	const int SIZE = 3;
	for (int i = 0; i < 12; i++) {
		cin >> ::data[i];
	}
	HANDLE hEvent = CreateEvent(NULL, FALSE, TRUE, NULL);

	HANDLE arhThreads[SIZE];
	for (int i = 0; i < SIZE; i++)
	{
		DWORD dwTheaID = 0;
		arhThreads[i] = CreateThread(NULL, 0, ThreadProc, hEvent, 0, &dwTheaID);
	}
	WaitForMultipleObjects(SIZE, arhThreads, TRUE, INFINITE);
	CloseHandle(hEvent);
	for (int i = 0; i < SIZE; i++) {
		CloseHandle(arhThreads[i]);
	}
	cout << "รัวี: " << total << endl;
}

