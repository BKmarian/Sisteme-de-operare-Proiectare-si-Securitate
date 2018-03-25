#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <cmath>  
DWORD WINAPI ThreadFunc(LPVOID lpThreadParameter)
{
	int dwParam = *((int *)lpThreadParameter);
	double *sqr = new double();
	*sqr = sqrt(dwParam);
	return (DWORD) sqr;
}
int main()
{
	DWORD returnStatus = 0;
	DWORD param = 100;
	DWORD dwId = 0;
	HANDLE hThread = CreateThread(NULL, 0, ThreadFunc, &param, 0, &dwId);
	if (WaitForSingleObject(hThread, INFINITE) == WAIT_OBJECT_0)
	{
		GetExitCodeThread(hThread, &returnStatus);
		double val = * (double *) returnStatus;
		printf("%f", val);
	}
	std::cin.get();
	CloseHandle(hThread);
	return 0;
}