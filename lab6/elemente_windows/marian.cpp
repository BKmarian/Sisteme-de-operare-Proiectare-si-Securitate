#include <windows.h>
int main()
{
HANDLE hFile = INVALID_HANDLE_VALUE;
while (1)
{
hFile = CreateFile("leak.txt", GENERIC_READ, FILE_SHARE_READ |
FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL,
OPEN_EXISTING, 0, NULL);
Sleep(1000);
}
CloseHandle(hFile);
}