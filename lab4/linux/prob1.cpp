#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{

	const char program[256] = "./p1";
	const char * argv1[] = {program, NULL}; 
	const char * argv2[] = {program, "param1", NULL}; 

	pid_t pid = fork();

	if (pid == -1) {
	  printf("error creating child process");
	  return -1;
	}
	else if (pid == 0) {
	  execvp(program, (char * const *)argv1); 
	}
	else {
	  int status;
	  if (waitpid(pid, &status, 0) == -1) {
	    printf("error waiting for child");
	    return -1;
	  }
	  else {
	    if (WIFEXITED(status))
			printf("Procesul %d asteptat, s-a terminat normal, cu codeul %d\n", pid, WEXITSTATUS(status)); 
	  }
	}

	pid = fork();
	if (pid == -1) {
	  printf("error creating child process");
	  return -1;
	}
	else if (pid == 0) {
	  execvp(program, (char * const *)argv2); 
	}
	else {
	  int status2;
	  if (waitpid(pid, &status2, 0) == -1) {
	    printf("error waiting for child");
	    return -1;
	  }
	  else {
	    if (WIFEXITED(status2))
			printf("\nProcesul %d asteptat, s-a terminat normal, cu codeul %d\n", pid, WEXITSTATUS(status2)); 
	  }
	}
	
	return 0;
}
