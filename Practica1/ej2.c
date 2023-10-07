#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>


int main(void)
{
	pid_t pid, childpid;
	int status;

	pid = fork();

	if(pid==-1)
	{
		perror("fork error");
		printf("errno value= %d\n", errno);
		exit(EXIT_FAILURE);
	}
	else if(pid==0)//hijo
	{
		printf("Hijo con pid: %d\n", getpid());
		exit(EXIT_SUCCESS);
	}
	else
	{
		printf("Padre con pid: %d\n", getpid());
		sleep(20);
		while ((childpid = wait(&status)) > 0)
		{
			if (WIFEXITED(status)) 
			{
				printf("Hijo ID:%ld finalizado, estado=%d\n", (long int) childpid, WEXITSTATUS(status));
			} 
			else if (WIFSIGNALED(status)) {  //Para seniales como las de finalizar o matar
				printf("Hijo  ID:%ld finalizado al recibir la señal %d\n", (long int) childpid, WTERMSIG(status));
			} 	  
		}//while

		if (childpid==(pid_t)-1 && errno==ECHILD)
		{
			printf("No hay más hijos que esperar\n");
			printf("Valor de errno=%d, definido como %s\n", errno, strerror(errno));
			perror("Valor de ernno usando perror");
		}
		else
		{
			printf("Error en la invocacion de wait o la llamada ha sido interrumpida por una señal.\n");
			exit(EXIT_FAILURE);
		}
		sleep(10);
		exit(EXIT_SUCCESS);
	}
}