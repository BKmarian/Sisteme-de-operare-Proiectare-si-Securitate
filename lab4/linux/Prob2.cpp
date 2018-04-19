#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string>
#include <errno.h> 
#include <sys/types.h>
#include <errno.h>
#include <string.h>
//Pentru compilare g++ -pthread -std=c++11 Prob2.cpp -o prob2
// -std=c++11 deoarece sunt probleme cu functia to_string in alte versiuni

#define SEM_NAME "/MarianSem2"
#define INITIAL_VALUE 1
int fd;
sem_t *semaphore;
const char line = '\n';
void readFile() {
	int nr;
	char *array = (char*)malloc(50*sizeof(char));
	int i = 0;
	char ch;
	while(read(fd,&ch,1) != 0)
	{
		printf("%d",ch);
		array[i] = ch;
		i++;
	}
	array[i] = '\0';
	sscanf(array, "%d", &nr);
	int nr2 = nr;

	lseek(fd, 0, SEEK_END);
	if(fork()) {
		printf("Started parent\n");
		for (int i = 0; i < 1000000; i++) {
			nr++;
			printf("parent wrote %d\n",nr);
			std::string s = std::to_string(nr);
			sem_wait(semaphore);
			write(fd,&line,1);
			write(fd,s.c_str(),s.length());
			sem_post(semaphore);
		}
	}
	else {
		printf("Started child\n");
		for (int i = 0; i < 1000000; i++) {
			printf("child wrote %d\n",nr2);
			nr2++;
			std::string s = std::to_string(nr2);
			sem_wait(semaphore);
			write(fd,&line,1);
			write(fd,s.c_str(),s.length());
			sem_post(semaphore);
	}
	}
}
int main(int argc, char *argv[])
{	
	fd = open (argv[1], O_RDWR);


    if (fd == -1){
      printf ("Error opening the file");
      return -1;
    }

    semaphore = sem_open(SEM_NAME, O_CREAT | O_EXCL, 0644, INITIAL_VALUE);
    sem_unlink(SEM_NAME); //pentru a preveni existenta semaforului dupa terminarea proceselor
    if (semaphore == SEM_FAILED) {
        printf("cannot create semaphore with error %s\n",strerror(errno));
        exit(EXIT_FAILURE);
    }


	readFile();


	close(fd);
	sem_destroy(semaphore);
	return 0;
}