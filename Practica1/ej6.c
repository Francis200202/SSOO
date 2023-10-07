#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

int contador=-2;
void alarma(int seg);
unsigned int ring(int seg);

int main()
{

	if(signal(SIGALRM, alarma)==SIG_ERR)
	{
		perror("Signal error");
   	    printf("errno value= %d\n", errno);  
		exit(EXIT_FAILURE);
	}

	ring(5);
	ring(3);
	while(1)
	{
		ring(1);
	}

}

void alarma(int signo)
{
	printf("RINGGG!!!\n");
	contador++;
	if(contador==4)
	{
		kill(getpid(), SIGKILL);
	}
	return;	/* nothing to do, just return to wake up the pause */
}

unsigned int ring(int seg)
{
	alarm(seg);		/* start the timer */
	pause();			/* pause - suspend the thread until a signal is received */ 
}