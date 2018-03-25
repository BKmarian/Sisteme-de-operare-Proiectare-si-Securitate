#include <windows.h>
#include <iostream>
int main(int arc, TCHAR * argv[])
{
	char buf[50];
	LPVOID lpMsgBuf;
	DWORD byteRead = 0;
	DWORD dw_bytes_written = 0;
	BOOL returnStatus;
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	HANDLE hFile = CreateFile(argv[1],
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);


	if (hFile == INVALID_HANDLE_VALUE)
		std::cout << "Error opening the file";

	while (returnStatus = ReadFile(hFile, buf, 50, &byteRead, NULL)) {
		if (0 == byteRead)
		{
			break;
		}
		WriteFile(hStdOut, buf, byteRead, &dw_bytes_written, NULL);
	}

	CloseHandle(hFile);
	std::cin.get();
	return 0;
}