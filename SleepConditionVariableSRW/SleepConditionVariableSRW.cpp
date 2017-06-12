// SleepConditionVariableSRW.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>

CONDITION_VARIABLE gCond;
SRWLOCK gLock;

DWORD WINAPI wake(LPVOID)
{
	Sleep(15000);
	WakeAllConditionVariable(&gCond);
	return 0;
}

int main()
{
	InitializeConditionVariable(&gCond);
	InitializeSRWLock(&gLock);
	AcquireSRWLockExclusive(&gLock);
	CreateThread(0, 0, wake, 0, 0, 0);
	DWORD time = GetTickCount();
	BOOL rv = SleepConditionVariableSRW(&gCond, &gLock, 30000, 0);
	ReleaseSRWLockExclusive(&gLock);
	printf("elapsed time=%d\n", GetTickCount() - time);
	if (rv == FALSE) {
		char err[256];
		FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)err, 256, NULL);
		printf("err=%s\n", err);
		return rv;
	}
	printf("success\n");
	return 0;
}

