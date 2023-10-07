#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

void tratarSennal();

int main()
{

	pid_t pid, childpid;
	int status;

	pid=fork();

	switch(pid)
	{
		case -1:
			perror("fork error");
      		printf("errno value= %d\n", errno); //Tipo de error
      		exit(EXIT_FAILURE);

      	case 0:
      		if(signal(SIGUSR1, tratarSennal)==SIG_ERR)
      		{
      			perror("Signal error");
   	  			printf("errno value= %d\n", errno);  
				exit(EXIT_FAILURE);
      		}
      		for(int i=0;i<5;i++)
      		{
      			pause();
      		}
      		exit(EXIT_SUCCESS);

      	default: //padre
      		for(int i=0;i<5;i++)
      		{
      			sleep(1);
      			kill(pid, SIGUSR1);
      		}

			while ( (childpid=wait(&status)) > 0 ) 
			{
				if (WIFEXITED(status)) 
				{
					printf("Proceso padre %d, hijo con PID %ld finalizado, status = %d\n", getpid(), (long int)childpid, WEXITSTATUS(status));
				} 
				else if (WIFSIGNALED(status)) //Para seniales como las de finalizar o matar
				{
					printf("Proceso padre %d, hijo con PID %ld finalizado al recibir la señal %d\n", getpid(), (long int)childpid, WTERMSIG(status));
				} 
			}
			if (childpid==(pid_t)-1 && errno==ECHILD) //Entra cuando vuelve al while y no hay más hijos que esperar
			{
				printf("Proceso padre %d, no hay mas hijos que esperar. Valor de errno = %d, definido como: %s\n", getpid(), errno, strerror(errno));
			}	
			else
			{
				printf("Error en la invocacion de wait o waitpid. Valor de errno = %d, definido como: %s\n", errno, strerror(errno));
				exit(EXIT_FAILURE);
			}
		exit(EXIT_SUCCESS);
	}

}

void tratarSennal()
{
	printf("VIVA ER BETI!!-_- =_=\n");
}