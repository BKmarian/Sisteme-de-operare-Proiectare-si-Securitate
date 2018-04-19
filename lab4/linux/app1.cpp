#include <stdlib.h>
#include <stdio.h>

int main(int argv, char *argc[]) {
	if (argv != 2)
		exit(-1);
	else {
		printf("%s", argc[1]);
		exit(0);
	}
}