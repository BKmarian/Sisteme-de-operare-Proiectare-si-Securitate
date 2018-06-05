#include <stdio.h>
#include "pers.h"
#include <windows.h>

int main()
{
	
	int t = 0;
	Pers a;
	a.age = 35;
	strcpy_s(a.cnp, CNP_SIZE, "1990115221544");
	a.nume = (char *) malloc(256);
	strcpy_s(a.nume, 256, "Numele Meu");
	t++;
	insert_pers(a);
	return 0;
}
