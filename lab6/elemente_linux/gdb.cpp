#include <stdio.h>
#include "pers.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int * myerr = NULL;
int main()
{
	myerr = __errno_location();
	int t = 0;
	Pers a;
	a.age = 35;
	strcpy(a.cnp, "1990115221544");
	a.nume = (char *) malloc(100);
	strcpy(a.nume, "Numele Meu");
	t++;
	insert_pers(a);
	return 0;
}
