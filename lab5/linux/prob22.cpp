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

    // ftok to generate unique key
    key_t key = ftok("shmfile2",65);
 
    // shmget returns an identifier in shmid
    int shmid = shmget(key,1024,0666|IPC_CREAT);
 
    // shmat to attach to shared memory
    char *str = (char*) shmat(shmid,NULL,0);
 
    for(int i = 1 ; i <= 9 ; i = i + 2) {
    	sem_wait(semaphore);
    	*(str + i - 1) = i +'0';
    	sem_post(semaphore);
    }
    getchar();
    printf("Memory Shared: %s\n",str);
    //detach from shared memory 
    shmdt(str);
   	
    // destroy the shared memory
    shmctl(shmid,IPC_RMID,NULL);
    
    return 0;
}