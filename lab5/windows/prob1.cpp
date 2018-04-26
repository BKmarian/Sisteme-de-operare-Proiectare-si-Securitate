#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
LONG CALLBACK VectoredExceptionHandler(PEXCEPTION_POINTERS exception)
{
	char *addr = (char *)exception->ExceptionRecord->ExceptionInformation[1];
	DWORD res;
	printf("%p\n", (void*)addr);
	DWORD rc = VirtualProtect(addr, 4096, PAGE_READWRITE, &res);
	printf("increased permissions %ld\n", rc);
	return EXCEPTION_CONTINUE_SEARCH;
}
int main() {
	char *p;
	p = (char *)VirtualAlloc(NULL, 4096, MEM_COMMIT, PAGE_READONLY);
	PVOID ret = AddVectoredExceptionHandler(1, VectoredExceptionHandler);
	if (ret == NULL) {
		printf("cannot create handler");
		system("pause");
		return -1;
	}
	printf("%p\n", (void*)p);
	*p = 'a';
	*(p + 1) = 'b';
	*(p + 2) = 'c';
	printf("1");
	printf("p = %s", p);
	system("pause");
	VirtualFree(p, 4096, MEM_DECOMMIT);
}