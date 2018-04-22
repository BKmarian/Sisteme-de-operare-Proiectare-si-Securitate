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
	char array[50];
	int i = 0;
	char ch;
	while(read(fd,&ch,1) != 0)
	{
		printf("%d",ch);
		if(ch == line) break;
		array[i] = ch;
		i++;
	}
	array[i] = '\0';
	sscanf(array, "%d", &nr);
	nr++;

	std::string s = std::to_string(nr);

	for (int i = 0; i < 1000000; i++) {
		printf("process wrote %d\n",nr);
		sem_wait(semaphore);
		lseek(fd, 0, SEEK_END);
		write(fd,&line,1);
		write(fd,s.c_str(),s.length());
		sem_post(semaphore);
	}
}
int main(int argc, char *argv[])
{	
	fd = open (argv[1], O_RDWR);


    if (fd == -1){
      printf ("Error opening the file");
      return -1;
    }

    semaphore = sem_open(SEM_NAME, O_CREAT, 0644, INITIAL_VALUE);
    if (semaphore == SEM_FAILED) {
        printf("cannot create semaphore with error %s\n",strerror(errno));
        return -1;
    }

    //sem_unlink(SEM_NAME); //pentru a preveni existenta semaforului dupa terminarea proceselor

	readFile();


	close(fd);
	sem_destroy(semaphore);
	return 0;
}