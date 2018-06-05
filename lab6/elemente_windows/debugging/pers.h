#ifndef __PERS_H__
#define __PERS_H__
#define CNP_SIZE 31
typedef struct __Pers
{
	int age;
	char cnp[CNP_SIZE];
	char * nume;

}Pers;


int insert_pers(Pers a);
#endif
