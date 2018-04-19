// C program to implement one side of FIFO
// This side writes first, then reads
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/file.h>
#include <errno.h>

int main()
{
    int fd;
    int processId = getpid();
    // FIFO file path
    const char * myfifo = "/tmp/myfifo2";
 
    // Creating the named file(FIFO)
    // mkfifo(<pathname>, <permission>)
    mkfifo(myfifo, 0666);
    
    int nr;
    while (1)
    {

        scanf("%d", &nr);
        fd = open(myfifo, O_WRONLY);
        //blochez operatiile pe file descriptor pentru a nu scrie un proces nr apoi alt proces nr si processId
        flock(fd,LOCK_EX);
        write(fd, &nr, sizeof(nr));
        write(fd, &processId, sizeof(processId));
        close(fd);
 
        // Open FIFO for Read only
        fd = open(myfifo, O_RDONLY);
 
        int sum = 0;
        read(fd, &sum, sizeof(sum));
        flock(fd,LOCK_UN);
        
        printf("Recieved sum: %d\n", sum);
        close(fd);
    }
    printf("cannot create semaphore with error %s\n",strerror(errno));
    return 0;
}