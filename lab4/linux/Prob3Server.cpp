#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <map>
#include <sys/stat.h> 
int main()
{
    int fd1;
 	std::map<int,int> sumMap;
    // FIFO file path
    const char * myfifo = "/tmp/myfifo2";
 
    // Creating the named file(FIFO)
    // mkfifo(<pathname>,<permission>)
    mkfifo(myfifo, 0666);
    while (1)
    {
        // First open in read only and read
        fd1 = open(myfifo,O_RDONLY);
        int nr,processId;
        read(fd1, &nr, sizeof(nr));
 		read(fd1, &processId, sizeof(processId));

        printf("Recieved: %d from process with id = %d\n", nr,processId);
        if(sumMap[processId] != NULL)
        	sumMap[processId] = sumMap[processId] + nr;
        else
        	sumMap[processId] = nr;

        close(fd1);
 
        // Now open in write mode and write
        // string taken from user.
        fd1 = open(myfifo,O_WRONLY);

        write(fd1, &sumMap[processId], sizeof(sumMap[processId]));
        close(fd1);
    }
    return 0;
}