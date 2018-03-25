#include<stdio.h>
#include<windows.h>
#include <queue>
#include <string.h>
std::queue<std::string> coada;
HANDLE hThreads[4];
HANDLE ghSemaphore;
CRITICAL_SECTION csSync;

DWORD WINAPI afisare(LPVOID lpThreadParameter) {
	int threadId = (int)lpThreadParameter;
	DWORD dwWaitResult1,dwWaitResult2;
	while (true) {
		dwWaitResult1 = WaitForSingleObject(ghSemaphore,INFINITE);
	

		EnterCriticalSection(&csSync);
		std::string numeFisier = coada.front();
		if (numeFisier == "abc") {
			coada.pop();
			LeaveCriticalSection(&csSync);
			break;
		}
		printf("%s thread id : %d\n", numeFisier.c_str(), threadId);
		coada.pop(); 
		LeaveCriticalSection(&csSync);
	}
	return NULL;
}

void dirListFiles(char* startDir, int depth)
{
	HANDLE hFind;
	WIN32_FIND_DATA content;
	char path[255];
	sprintf_s(path, "%s\\*", startDir);


	if ((hFind = FindFirstFile(path, &content)) == INVALID_HANDLE_VALUE)
	{
		abort();
	}

	BOOL cont = TRUE;
	while (cont == TRUE)
	{
		if ((strcmp(".", content.cFileName) != 0) && (strcmp("..", content.cFileName) != 0))
		{
			if (content.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				EnterCriticalSection(&csSync);
				std::string numeFisier(content.cFileName);
				coada.push(numeFisier);
				//printf("%s\n", content.cFileName);
				ReleaseSemaphore(ghSemaphore, 1, NULL);
				LeaveCriticalSection(&csSync);
				sprintf_s(path, "%s\\%s", startDir, content.cFileName);
				dirListFiles(path, depth + 1);
			}
			else
			{
				EnterCriticalSection(&csSync);
				std::string numeFisier(content.cFileName);
				coada.push(content.cFileName);
				ReleaseSemaphore(ghSemaphore, 1, NULL);
				LeaveCriticalSection(&csSync);
			}
		}
		cont = FindNextFile(hFind, &content);
	}
}

void closeHandles() {
	DeleteCriticalSection(&csSync);
	CloseHandle(ghSemaphore);
	for (int i = 0; i < 4; ++i)
		CloseHandle(hThreads[i]);
}

int main(int argc, char** argv)
{
	char* startDir = "c:\\work";
	DWORD dwId1;

	void* result;
	
	InitializeCriticalSection(&csSync);

	ghSemaphore = CreateSemaphore(
		NULL,    
		0, 
		100, 
		NULL); 

	for (int i = 0; i <= 3; i++) {
		hThreads[i] = CreateThread(NULL, 0, afisare, (void*)i, 0, &dwId1);
	}

	dirListFiles(startDir, 0);
	coada.push("abc");
	coada.push("abc");
	coada.push("abc");
	coada.push("abc"); 
	ReleaseSemaphore(ghSemaphore, 4, NULL);

	DWORD ret = WaitForMultipleObjects(4, hThreads, TRUE, INFINITE);

	closeHandles();
	system("pause");
	return 0;
}
