

#include "windows.h"
#include<stdlib.h>
#include<stdio.h>

int main(int argc, char* argv[])
{

	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);

	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));

	DWORD dwErr;

	TCHAR program1[256] = "C:\\Users\\marian\\Documents\\visual studio 2015\\Projects\\lab4\\lab4\\app1";
	TCHAR program2[256] = "C:\\Users\\marian\\Documents\\visual studio 2015\\Projects\\lab4\\lab4\\app1 marian";
	BOOL bResult = CreateProcess(NULL, program1, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if (!bResult) {
		dwErr = GetLastError();
		printf(("Create process failed, error=%x\n"), dwErr);
	}
	else {
		DWORD exitCode;
		WaitForSingleObject(pi.hProcess, INFINITE);
		GetExitCodeProcess(pi.hProcess,&exitCode);
		printf("%d\n",exitCode);
	}

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);

	ZeroMemory(&pi, sizeof(pi));

	BOOL bResult2 = CreateProcess(NULL, program2, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	
	if (!bResult2)
	{
		dwErr = GetLastError();
		printf(("Create process failed, error=%x\n"), dwErr);
	}
	else {
		DWORD exitCode;
		WaitForSingleObject(pi.hProcess, INFINITE);
		GetExitCodeProcess(pi.hProcess, &exitCode);
		printf("%d\n", exitCode);
	}

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	
	return 0;
}
