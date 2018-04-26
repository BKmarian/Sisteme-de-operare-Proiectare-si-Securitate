 
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

#define SEM_NAME "/MarianSem223"
#define INITIAL_VALUE 1

sem_t *semaphore;

int main()
{
	semaphore = sem_open(SEM_NAME, O_CREAT, 0644, INITIAL_VALUE);
    if (semaphore == SEM_FAILED) {
        printf("cannot create semaphore with error ");
        return -1;
    }

    key_t key = ftok("shmfile2",65);
 

    int shmid = shmget(key,1024,0666|IPC_CREAT);
 
    char *str = (char*) shmat(shmid,NULL,0);
 
    for(int i = 2 ; i <= 10 ; i = i + 2) {
    	sem_wait(semaphore);
    	if(i >= 10) {
    		*(str + i - 1) = (char)i%10;
    		*(str + i) = (char)i/10;
    	}
    	else
			*(str + i - 1) = i + '0';
    	sem_post(semaphore);
    }
    getchar();
    printf("Memory Shared: %s\n",str); 
    //detach from shared memory 
    shmdt(str);
 
    return 0;
}