#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

void pidd(int signo);

int main()
{

	if(signal(SIGUSR1, pidd)==SIG_ERR)
			{
				perror("Signal error");
   	  			printf("errno value= %d\n", errno);  
				exit(EXIT_FAILURE);
			}
			
	while(1){}
	exit(EXIT_SUCCESS);
}

void pidd(int signo)
{
	printf("Mi pid es: %d\n", getpid());
}