#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
int main()
{
	HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);
	HANDLE hPipe;
	DWORD dwWritten, dwRead;
	int sum;
	char temp[50];
	hPipe = CreateFile(TEXT("\\\\.\\pipe\\Pipe"),
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);

	if (hPipe != INVALID_HANDLE_VALUE)
	{
		while (1!=0) {
			ReadFile(hStdIn,&temp,4,&dwRead,NULL);
			if (strncmp(temp, "exit",4) == 0) {
				CloseHandle(hPipe);
				return 0;
			}
			int tempInt = atoi(temp);
			WriteFile(hPipe, &tempInt, 4, &dwWritten, NULL);


			if (ReadFile(hPipe, &sum, sizeof(int), &dwRead, NULL) != FALSE) {
				printf("Recieved sum = %d\n", sum);
			}
			else
				printf("Eroare la citirea sumei");
		}
	}
	system("pause");
	CloseHandle(hPipe);
	return 0;
}