
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
HANDLE hFile, hMutexOne,hEvent;
const char line = '\n';
void readFile() {
	DWORD dw_bytes_written, byteRead, dwPtrLow;

	char *array = (char*)malloc(50 * sizeof(char));
	int i = 0,nr;
	char ch;
	while (1)
	{
		ReadFile(hFile, &ch, 1, &byteRead, NULL);
		if (byteRead == 0) break;
		array[i] = ch;
		i++;
	}
	array[i] = '\0';
	sscanf_s(array, "%d", &nr);
	nr++;
	printf("nr = %d", nr);
	WaitForSingleObject(hEvent, INFINITE);
	for (int i = 0; i < 1000000; i++) {
		printf("%d", i);
		std::string s = std::to_string(nr);
		WaitForSingleObject(hMutexOne, INFINITE);
		
		dwPtrLow = SetFilePointer(hFile,
			0,
			NULL,
			FILE_END);

		WriteFile(hFile, &line, 1, &dw_bytes_written, NULL);
		WriteFile(hFile, s.c_str(), s.length(), &dw_bytes_written, NULL);

		ReleaseMutex(hMutexOne);
	}
}
int main(int argc, char *argv[])
{
	LPDWORD temp = NULL;
	hFile = CreateFile("C:\\Users\\marian\\Desktop\\SO\\lab4\\windows\\Text.txt",
		GENERIC_WRITE | GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);


	if (hFile == INVALID_HANDLE_VALUE)
		WriteFile(hStdOut, "Error opening the file", sizeof("Error opening the file"), temp, NULL);

	TCHAR *tszMutexName = "AMutexMarian";

	hMutexOne = CreateMutex(NULL, FALSE, tszMutexName);
	if (hMutexOne == NULL) {
		printf("Unable to create mutex.\n");
		printf("Error no. %d\n", GetLastError());
		exit(EXIT_FAILURE);
	}
	hEvent = CreateEvent(NULL, false, false, "MyEvent");
	
	if (hEvent == NULL) {
		printf("Unable to create event.\n");
		printf("Error no. %d\n", GetLastError());
		exit(EXIT_FAILURE);
	}

	if (hMutexOne == NULL) {
		printf("Unable to open mutex.\n");
		exit(EXIT_FAILURE);
	}


	readFile();


	CloseHandle(hMutexOne);
	CloseHandle(hFile);
	CloseHandle(hEvent);
	return 0;
}