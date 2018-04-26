#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>

#pragma comment(lib, "user32.lib")

#define BUF_SIZE 256
TCHAR szName[] = TEXT("MyFileMappingObject");

int _tmain()
{
	HANDLE hMutexOne = CreateMutex(NULL, FALSE, "mutexx");
	if (hMutexOne == NULL) {
		printf("Unable to open mutex.\n");
		CloseHandle(hMutexOne);
		return -1;
	}
	HANDLE hMapFile;
	LPCTSTR pBuf;

	hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,    // use paging file
		NULL,                    // default security
		PAGE_READWRITE,          // read/write access
		0,                       // maximum object size (high-order DWORD)
		BUF_SIZE,                // maximum object size (low-order DWORD)
		szName);                 // name of mapping object

	if (hMapFile == NULL)
	{
		_tprintf(TEXT("Could not create file mapping object (%d).\n"),
			GetLastError());
		return 1;
	}
	pBuf = (LPTSTR)MapViewOfFile(hMapFile,   // handle to map object
		FILE_MAP_ALL_ACCESS, // read/write permission
		0,
		0,
		BUF_SIZE);

	if (pBuf == NULL)
	{
		_tprintf(TEXT("Could not map view of file (%d).\n"),
			GetLastError());

		CloseHandle(hMapFile);

		return 1;
	}
	char* buf = (char *)pBuf;
	for (int i = 1; i <= 9; i = i + 2) {
		WaitForSingleObject(hMutexOne, INFINITE);
		if(i >= 10) {
    		*(buf + i - 1) = ((int)i/10) + '0';
    		*(buf + i) = ((int)i%10) + '0';
    	}
    	else
			*(buf + i - 1) = i + '0';
		ReleaseMutex(hMutexOne);
	}

	system("pause");
	printf("Shared: %s", buf);
	system("pause");
	UnmapViewOfFile(pBuf);
	CloseHandle(hMapFile);
	CloseHandle(hMutexOne);
	return 0;
}