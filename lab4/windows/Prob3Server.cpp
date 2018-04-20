#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
int main()
{
	HANDLE hPipe;
	char buffer[1024];
	DWORD dwRead, dwWrite;
	int sum = 0,temp;

	hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\Pipe"),
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
		1,
		1024 * 16,
		1024 * 16,
		NMPWAIT_USE_DEFAULT_WAIT,
		NULL);
	while (hPipe != INVALID_HANDLE_VALUE)
	{
		if (ConnectNamedPipe(hPipe, NULL) != FALSE)
		{
			while (ReadFile(hPipe, &temp, 4, &dwRead, NULL) != FALSE)
			{

				sum += temp;


				WriteFile(hPipe,&sum,4 , &dwWrite, NULL);

				printf("Sent sum = %d\n", sum);
			}
		}

		DisconnectNamedPipe(hPipe);
	}

	CloseHandle(hPipe);
	return 0;
}