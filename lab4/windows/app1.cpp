#include "windows.h"
#include<stdlib.h>
#include<stdio.h>

int main(int argv, char *argc[]) {
	if (argv != 2)
		return -1;
	else {
		printf("%s", argc[1]);
		return 0;
	}
}