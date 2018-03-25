#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <cmath>  
int global = 0;
HANDLE ghMutex;
HANDLE hThreads[2];
DWORD WINAPI ThreadFunc(LPVOID lpThreadParameter)
{
	DWORD dwWaitResult;

	for (int i = 0; i < 1000000; i++) {

		dwWaitResult = WaitForSingleObject(
			ghMutex,
			INFINITE);

		global++;
		ReleaseMutex(ghMutex);
	}
	return NULL;
}
void closeHandles() {
	CloseHandle(ghMutex);
	for (int i = 0; i < 2; ++i)
		CloseHandle(hThreads[i]);
}
int main()
{
	DWORD returnStatus = 0;
	DWORD dwId1,dwId2;

	ghMutex = CreateMutex(
		NULL,            
		FALSE,            
		NULL);            

	if (ghMutex == NULL)
	{
		printf("CreateMutex error: %d\n", GetLastError());
		return 1;
	}

	 hThreads[0] = CreateThread(NULL, 0, ThreadFunc, NULL, 0, &dwId1);
	 hThreads[1] = CreateThread(NULL, 0, ThreadFunc, NULL, 0, &dwId2);
	 DWORD ret = WaitForMultipleObjects(2, hThreads, TRUE, INFINITE);
	 if (ret != WAIT_OBJECT_0) {
		printf("Thread blocat");
		closeHandles();
		return 0;
	 }
	 printf("%d", global);
	
	 std::cin.get();
	 closeHandles();

	 return 0;
}