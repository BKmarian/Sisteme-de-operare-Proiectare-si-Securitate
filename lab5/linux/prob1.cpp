 
#include <stdio.h>
#include <signal.h>
#include <sys/mman.h>
#include <string.h>
void handler(int sig, siginfo_t *si, void *unused)
{
 printf("SIGSEGV la adresa: %p\n",(char *) si->si_addr);
 mprotect(si->si_addr, 4096, PROT_WRITE);
}
int main()
{
 struct sigaction sa;

 char *addr = (char *)mmap(NULL, 4096, PROT_NONE, MAP_SHARED | MAP_ANONYMOUS, 0, 0);
 printf("%p\n",addr);
 memset(&sa, 0, sizeof(struct sigaction));
 sa.sa_flags = SA_SIGINFO;

 sa.sa_sigaction = handler; 
 if (sigaction(SIGSEGV, &sa, NULL) == -1) {
 	munmap(addr,4096);
 	printf("sigaction error");
 	return -1;
 }
 
*addr= 's';
 *(addr+1) = 'a';
 *(addr+2) = 'm';
 
 printf("virtual mem: %s\n",addr);
 munmap(addr,4096);
}