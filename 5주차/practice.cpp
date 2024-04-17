#include <stdio.h>
#include <windows.h>
#include <process.h>
#include <tchar.h>

#define NUM_OF_GATE		4
#define SIZE	12

int index = -1;
int data[12];
LONG gTotal = 0;

CRITICAL_SECTION   gCriticalSection;
HANDLE	hMutex;

void AddData()
{
	EnterCriticalSection (&gCriticalSection);
	
	int start = index * 4;
	int end = (index + 1) * 4 - 1;

	for (int i = start; i <= end; i++)
	{
		gTotal += data[i];
	}

	LeaveCriticalSection (&gCriticalSection);
}

unsigned int WINAPI ThreadProc(LPVOID lpParam)
{
	WaitForSingleObject(hMutex, INFINITE);
	if (index == -1) {
		for (int i = 0; i < SIZE; i++)
		{
			scanf_s("%d", data + i);
		}
	}
	else {
		AddData();
	}
	index++;
	ReleaseMutex(hMutex);

	return 0;
}


int _tmain(int argc, TCHAR* argv[])
{
	DWORD dwThreadIDs[NUM_OF_GATE];
	HANDLE hThreads[NUM_OF_GATE];

	InitializeCriticalSection(&gCriticalSection);
	hMutex = CreateMutex(
		NULL,     // 디폴트 보안관리자.
		FALSE,    // 누구나 소유 할 수 있는 상태로 생성.
		NULL      // numaned mutex
	);

	if (hMutex == NULL)
	{
		_tprintf(_T("CreateMutex error: %d\n"), GetLastError());
	}

	for (DWORD i = 0; i < NUM_OF_GATE; i++)
	{
		hThreads[i] = (HANDLE)
			_beginthreadex(
				NULL,
				0,
				ThreadProc,
				NULL,
				CREATE_SUSPENDED,
				(unsigned*)&dwThreadIDs[i]
			);

		if (hThreads[i] == NULL)
		{
			_tprintf(_T("Thread creation fault! \n"));
			return -1;
		}
	}

	for (DWORD i = 0; i < NUM_OF_GATE; i++)
	{
		ResumeThread(hThreads[i]);
	}

	WaitForMultipleObjects(NUM_OF_GATE, hThreads, TRUE, INFINITE);

	_tprintf(_T("total: %d \n"), gTotal);

	for (DWORD i = 0; i < NUM_OF_GATE; i++)
	{
		CloseHandle(hThreads[i]);
	}

	DeleteCriticalSection(&gCriticalSection);
	CloseHandle(hMutex);

	return 0;
}
