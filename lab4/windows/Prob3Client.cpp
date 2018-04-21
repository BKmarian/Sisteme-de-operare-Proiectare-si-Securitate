#include <windows.h> 
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <process.h>

#define BUFSIZE 512

int _tmain(int argc, TCHAR *argv[])
{
	OVERLAPPED structOverlap;
	ZeroMemory(&structOverlap, sizeof(structOverlap));
	int sum, temp;
	HANDLE hPipe;
	LPTSTR lpvMessage = TEXT("Default message from client.");
	TCHAR  chBuf[BUFSIZE];
	BOOL   fSuccess = FALSE, fSuccess2 = FALSE;
	DWORD  cbRead, cbToWrite, cbWritten, dwMode, dwWritten, dwRead;;
	LPTSTR lpszPipename = TEXT("\\\\.\\pipe\\mynamedpipe");
	int pid = _getpid();

	if (argc > 1)
		lpvMessage = argv[1];

	// Try to open a named pipe; wait for it, if necessary. 

	while (1)
	{
		hPipe = CreateFile(
			lpszPipename,   // pipe name 
			GENERIC_READ |  // read and write access 
			GENERIC_WRITE,
			0,              // no sharing 
			NULL,           // default security attributes
			OPEN_EXISTING,  // opens existing pipe 
			0,              // default attributes 
			NULL);          // no template file 

							// Break if the pipe handle is valid. 

		if (hPipe != INVALID_HANDLE_VALUE)
			break;

		// Exit if an error other than ERROR_PIPE_BUSY occurs. 

		if (GetLastError() != ERROR_PIPE_BUSY)
		{
			_tprintf(TEXT("Could not open pipe. GLE=%d\n"), GetLastError());
			return -1;
		}

		// All pipe instances are busy, so wait for 20 seconds. 

		if (!WaitNamedPipe(lpszPipename, 20000))
		{
			printf("Could not open pipe: 20 second wait timed out.");
			return -1;
		}
	}

	dwMode = PIPE_READMODE_MESSAGE;
	fSuccess = SetNamedPipeHandleState(
		hPipe,    // pipe handle 
		&dwMode,  // new pipe mode 
		NULL,     // don't set maximum bytes 
		NULL);    // don't set maximum time 
	if (!fSuccess)
	{
		_tprintf(TEXT("SetNamedPipeHandleState failed. GLE=%d\n"), GetLastError());
		return -1;
	}

	// MyCode
	while (1) {

		scanf_s("%d", &temp);
		LockFileEx(hPipe, LOCKFILE_EXCLUSIVE_LOCK | LOCKFILE_FAIL_IMMEDIATELY, 0, MAXDWORD, MAXDWORD, &structOverlap); //lock pe fisier pentru a evita cazul in care s-ar putea intercala scrierileintre 2 procese (ex : pid1 pid2 nr1 nr2)
		fSuccess = WriteFile(hPipe, &pid, sizeof(pid), &dwWritten, NULL);
		fSuccess2 = WriteFile(hPipe, &temp, sizeof(temp), &dwWritten, NULL);
		if (!fSuccess2 || !fSuccess)
		{
			_tprintf(TEXT("WriteFile to pipe failed. GLE=%d\n"), GetLastError());
			system("pause");
			CloseHandle(hPipe);
			return -1;
		}
		printf("recieved procId = %d , nr = %d\n", pid, temp);

		fSuccess = ReadFile(hPipe, &sum, sizeof(sum), &dwRead, NULL); 
		if (!fSuccess)
		{
			_tprintf(TEXT("ReadFile to pipe failed. GLE=%d\n"), GetLastError());
			system("pause");
			CloseHandle(hPipe);
		}
		UnlockFileEx(hPipe, 0, MAXDWORD, MAXDWORD, &structOverlap);
		printf("recieved sum = %d\n", sum);
	}

	CloseHandle(hPipe);
	printf("GLE = %", GetLastError());
	system("pause");
	return 0;
}