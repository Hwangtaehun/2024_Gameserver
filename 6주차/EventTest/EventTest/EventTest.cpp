#include "stdafx.h"
#include "Windows.h"
#include "iostream"
using namespace std;

int data[12];
int total = 0;
float avg = 0;
bool write = false;
bool read = false;

DWORD WINAPI ThreadInout(LPVOID pParam)
{
	HANDLE hEvent = (HANDLE)pParam;

	DWORD dwWaitCode = WaitForSingleObject(hEvent, INFINITE);
	if (dwWaitCode == WAIT_FAILED)
	{
		cout << "WaitForSingleObject failed : " << GetLastError() << endl;
		return 0;
	}

	if (!write) {
		for (int i = 0; i < 12; i++) {
			cin >> ::data[i];
		}
		write = true;
	}
	else {
		for (int i = 0; i < 12; i++) {
			cout << ::data[i] << " ";
		}
		cout << endl;
		read = true;
	}
	
	SetEvent(hEvent);
	return 0;
}

DWORD WINAPI ThreadProc(LPVOID pParam)
{
	HANDLE hMutex = (HANDLE)pParam;
	DWORD dwWaitCode = WaitForSingleObject(hMutex, INFINITE);
	if (dwWaitCode == WAIT_FAILED)
	{
		cout << "WaitForSingleObject failed : " << GetLastError() << endl;
		return 0;
	}
	if (dwWaitCode == WAIT_ABANDONED)
		cout << "Abandoned Mutex acquired!!!" << GetLastError() << endl;


	if (write) {
		for (int i = 0; i < 12; i++) {
			total += ::data[i];
		}
		write = false;
	}
	else {
		avg = (float)total / 12;
	}

	ReleaseMutex(hMutex);
	return 0;
}

void _tmain(void)
{
	const int SIZE = 2;
	HANDLE hEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	HANDLE arhThreads[SIZE];

	for (int i = 0; i < SIZE; i++)
	{
		DWORD dwTheaID = 0;
		arhThreads[i] = CreateThread(NULL, 0, ThreadInout, hEvent, 0, &dwTheaID);
	}
	WaitForMultipleObjects(SIZE, arhThreads, TRUE, INFINITE);
	CloseHandle(hEvent);
	for (int i = 0; i < SIZE; i++) {
		CloseHandle(arhThreads[i]);
	}

	while (true)
	{
		if (read) {
			HANDLE hMutex = CreateMutex(NULL, FALSE, NULL);
			for (int i = 0; i < SIZE; i++)
			{
				DWORD dwTheaID = 0;
				arhThreads[i] = CreateThread(NULL, 0, ThreadProc, hMutex, 0, &dwTheaID);
			}
			WaitForMultipleObjects(SIZE, arhThreads, TRUE, INFINITE);
			CloseHandle(hMutex);
			for (int i = 0; i < SIZE; i++) {
				CloseHandle(arhThreads[i]);
			}
			break;
		}
	}
	cout << "ÃÑÇÕ = " << total << ", Æò±Õ = " << avg << endl;
}

